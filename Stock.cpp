#include <iostream>                             // Include standard input-output stream library
#include <fstream>                              // Include file stream library for file operations
#include <iomanip>                             // Include library for manipulating output formats
#include <stdexcept>                           // Include library for standard exceptions
#include "stock.hpp"                           // Include the header file for Stock class
#include "utils/utils.hpp"                    // Include utility functions from utils folder
#include <nlohmann/json.hpp>                  // Include the nlohmann JSON library for JSON manipulation

using json = nlohmann::json;                  // Create an alias for nlohmann::json to simplify usage

// Updated constructor to match header
// Stock constructors
Stock::Stock() : stockID(""),                 // Default constructor initializes stockID to empty string
                 stockName(""),               // Initializes stockName to empty string
                 currentPrice(0.0),           // Initializes currentPrice to 0.0
                 fiftyTwoWeekHigh(0.0),       // Initializes fiftyTwoWeekHigh to 0.0
                 fiftyTwoWeekLow(0.0),        // Initializes fiftyTwoWeekLow to 0.0
                 userID("") {}                 // Initializes userID to empty string

Stock::Stock(string stockID, string stockName, double currentPrice, string userId) 
    : stockID(stockID),                        // Constructor initializes stockID with provided value
      stockName(stockName),                    // Initializes stockName with provided value
      currentPrice(currentPrice),              // Initializes currentPrice with provided value
      fiftyTwoWeekHigh(0.0),                   // Initializes fiftyTwoWeekHigh to 0.0
      fiftyTwoWeekLow(0.0),                    // Initializes fiftyTwoWeekLow to 0.0
      userID(userId) {}                        // Initializes userID with provided value

// OwnedStock constructors
OwnedStock::OwnedStock() : Stock(),          // Default constructor initializes OwnedStock with Stock's default constructor
                           quantity(0),       // Initializes quantity to 0
                           buyPrice(0.0) {}   // Initializes buyPrice to 0.0

OwnedStock::OwnedStock(string stockID, string stockName, double currentPrice, 
                       string userId, int quantity, double buyPrice)
    : Stock(stockID, stockName, currentPrice, userId), // Initializes base Stock class with parameters
      quantity(quantity),                            // Initializes quantity with provided value
      buyPrice(buyPrice) {}                         // Initializes buyPrice with provided value

void Stock::setUser ID(const string& id) {         // Setter for userID
    userID = id;                                   // Assigns provided id to userID
}

string Stock::getUser ID() {                        // Getter for userID
    return userID;                                 // Returns current userID
}

string Stock::getstockname() {                     // Getter for stockName
    return stockName;                              // Returns current stockName
}

void Stock::setStockName(const string& name) {    // Setter for stockName
    stockName = name;                              // Assigns provided name to stockName
}

double Stock::getPrice() const {                   // Getter for currentPrice
    return currentPrice;                           // Returns current price of the stock
}

void Stock::setCurrentPrice(double price) {       // Setter for currentPrice
    currentPrice = price;                          // Assigns provided price to currentPrice
}

void Stock::displayDetails() {                     // Function to display stock details
    string command = "java -cp lib/json-20240303.jar searchapi.java " + stockID; // Command to get stock details
    system(command.c_str());                       // Executes the command in the system shell
}

// Updated buyStock implementation
void Stock::buyStock(string stockName, int quantity) { // Function to handle stock purchase
    json stockDetails;                               // JSON object to hold stock details
    ifstream stockFile("stockdetails.json");        // Open stock details file for reading
    if (!stockFile.good()) {                         // Check if file is accessible
        throw runtime_error("Error reading stock details"); // Throw error if file is not good
    }
    stockFile >> stockDetails;                       // Read stock details from the file
    stockFile.close();                               // Close the file

    setStockName(stockName);                         // Set the stock name
    auto stockInfo = stockDetails[getstockname() + ":NASDAQ"]["price_info"]; // Extract price info from JSON
    double currentPrice = stod(stockInfo["current_price"].get<string>()); // Convert current price to double
    const double USD_TO_INR = 83.0;                 // Define conversion rate from USD to INR
    double priceInRupees = currentPrice * USD_TO_INR; // Calculate price in rupees
    double totalPriceInRupees = priceInRupees * quantity; // Calculate total price for the quantity purchased

    cout << "Stock Purchase Details:\n";             // Print header for stock purchase details
    cout << "Current Price: " << currentPrice << " USD (Rs." << priceInRupees << ")\n"; // Display current price
    cout << "Quantity: " << quantity << "\n";       // Display quantity being purchased
    cout << "Total Cost: Rs." << totalPriceInRupees << "\n"; // Display total cost in rupees
    cout << "Confirm purchase? (y/n): ";            // Prompt user for purchase confirmation
    char choice;                                    // Variable to store user choice
    cin >> choice;                                  // Read user choice

    if (choice == 'y') {                            // If user confirms purchase
        User currentUser ;                           // Create a User object
        currentUser .setUser ID(userID);             // Set the userID for the current user
        double walletBalance = currentUser .checkWalletBalance(); // Check current wallet balance
        
        if (walletBalance >= totalPriceInRupees) { // Check if sufficient funds are available
            json userData;                          // JSON object to hold user data
            ifstream inFile("test.json");          // Open user data file for reading
            if (inFile.good()) {                    // Check if file is accessible
                inFile >> userData;                 // Read user data from the file
            }
            inFile.close();                         // Close the file

            // Update portfolio
            userData[userID]["portfolio"][stockName] = { // Update user's portfolio with new stock
                {"AveragePrice", currentPrice},    // Set average price in portfolio
                {"CurrentPrice", currentPrice},    // Set current price in portfolio
                {"Quantity", quantity}              // Set quantity in portfolio
            };

            // Record in stock_transactions section
            string transactionId = guuid();        // Generate a unique transaction ID
            string timestamp = gettime();          // Get the current timestamp
            userData["stock_transactions"][transactionId] = { // Record transaction details
                {"amount", totalPriceInRupees},    // Set transaction amount
                {"date", timestamp},                // Set transaction date
                {"paymentMethod", "Wallet"},        // Set payment method
                {"stock_name", stockName}           // Set stock name
            };

            ofstream outFile("test.json");         // Open user data file for writing
            outFile << setw(4) << userData << endl; // Write updated user data to the file
            outFile.close();                        // Close the file

            currentUser .removeMoneyfromWallet(totalPriceInRupees); // Deduct amount from user's wallet

            cout << "Successfully purchased " << quantity << " shares of " << stockName << "\n"; // Confirm purchase
            //cout << "Your updated wallet balance is Rs." << currentUser .checkWalletBalance() << "\n"; // Display updated balance
        } else {
            cout << "Insufficient funds. Current balance: Rs." << walletBalance << "\n"; // Notify insufficient funds
        }
    }
}

void Stock::saveStockDataToLocal(string stockName, int quantity) { // Function to save stock data locally
    json userData;                                  // JSON object to hold user data
    json stockDetails;                              // JSON object to hold stock details
    string timestamp = gettime();                   // Get the current timestamp
    string transactionId = guuid();                 // Generate a unique transaction ID

    // Load user data
    ifstream inFile("test.json");                   // Open user data file for reading
    if (inFile.good()) {                             // Check if file is accessible
        inFile >> userData;                          // Read user data from the file
    }
    inFile.close();                                  // Close the file

    // Load stock details and get current price
    ifstream stockFile("stockdetails.json");        // Open stock details file for reading
    if (stockFile.good()) {                          // Check if file is accessible
        stockFile >> stockDetails;                   // Read stock details from the file
    }
    stockFile.close();                               // Close the file

    double currentPrice = stod(stockDetails[stockName + ":NASDAQ"]["price_info"]["current_price"].get<string>()); // Get current price
    const double USD_TO_INR = 83.0;                 // Define conversion rate from USD to INR
    double priceInRupees = currentPrice * USD_TO_INR; // Calculate price in rupees

    // Update portfolio
    userData[userID]["portfolio"][stockName] = {   // Update user's portfolio with stock data
        {"AveragePrice", priceInRupees * quantity / quantity},  // Average price in INR
        {"CurrentPrice", priceInRupees},       // Current price in INR
        {"Quantity", quantity}                  // Quantity of stock
    };

    // Record in stock_transactions
    userData["stock_transactions"][transactionId] = { // Record transaction details
        {"amount", priceInRupees * quantity},         // Total amount for the transaction
        {"date", timestamp},                            // Date of the transaction
        {"paymentMethod", "Wallet"},                    // Payment method used
        {"stock_name", stockName}                       // Name of the stock
    };

    ofstream outFile("test.json");                     // Open user data file for writing
    outFile << setw(4) << userData << endl;            // Write updated user data to the file
    outFile.close();                                    // Close the file
}

void OwnedStock::sell(string stockid, int sellQuantity) { // Function to sell owned stock
    json userData;                                      // JSON object to hold user data
    ifstream inFile("test.json");                       // Open user data file for reading
    if (!inFile.good()) {                               // Check if file is accessible
        throw runtime_error("Error reading transactions"); // Throw error if file is not good
    }
    inFile >> userData;                                 // Read user data from the file
    inFile.close();                                     // Close the file

    // Check if user owns the stock and has sufficient quantity
    if (!userData[getUser ID()]["portfolio"].contains(stockid)) { // Check if stock is in portfolio
        cout << "You don't own any shares of " << stockid << "\n"; // Notify user if stock is not owned
        return;                                          // Exit function
    }

    int currentQuantity = userData[getUser ID()]["portfolio"][stockid]["Quantity"].get<int>(); // Get current quantity
    if (currentQuantity < sellQuantity) {              // Check if sufficient shares are available
        cout << "Insufficient shares. You own " << currentQuantity << " shares of " << stockid << "\n"; // Notify user
        return;                                          // Exit function
    }

    // Get current price
    string command = "java -cp lib/json-20240303.jar currentprice.java " + stockid; // Command to get current price
    system(command.c_str());                           // Execute the command in the system shell

    json stockDetails;                                  // JSON object to hold stock details
    ifstream stockFile("stockdetails.json");           // Open stock details file for reading
    if (stockFile.good()) {                            // Check if file is accessible
        stockFile >> stockDetails;                      // Read stock details from the file
    }
    stockFile.close();                                  // Close the file

    double updatedPrice = stod(stockDetails[stockid + ":NASDAQ"]["price_info"]["current_price"].get<string>()); // Get updated price
    const double USD_TO_INR = 83.0;                    // Define conversion rate from USD to INR
    double priceInRupees = updatedPrice * USD_TO_INR;  // Calculate price in rupees
    double saleProceeds = sellQuantity * priceInRupees; // Calculate total sale proceeds

    // Show sale details and get confirmation
    cout << "\nStock Sale Details:\n";                   // Print header for stock sale details
    cout << "Current Price: " << updatedPrice << " USD (Rs." << priceInRupees << ")\n"; // Display current price
    cout << "Quantity to Sell: " << sellQuantity << "\n"; // Display quantity to sell
    cout << "Total Sale Value: Rs." << saleProceeds << "\n"; // Display total sale value
    cout << "Confirm sale? (y/n): ";                    // Prompt user for sale confirmation
    
    char choice;                                        // Variable to store user choice
    cin >> choice;                                      // Read user choice
    if (choice != 'y') {                                // If user does not confirm sale
        cout << "Sale cancelled.\n";                    // Notify user of cancellation
        return;                                          // Exit function
    }

    User currentUser ;                                   // Create a User object
    currentUser .setUser ID(getUser ID());                // Set the userID for the current user

    // Update portfolio quantity
    int remainingQuantity = currentQuantity - sellQuantity; // Calculate remaining quantity
    if (remainingQuantity == 0) {                       // If quantity becomes 0
        userData[getUser ID()]["portfolio"].erase(stockid); // Remove stock entry completely
    } else {
        userData[getUser ID()]["portfolio"][stock id]["Quantity"] = remainingQuantity;               // Update quantity in portfolio
        userData[getUser  ID()]["portfolio"][stockid]["CurrentPrice"] = priceInRupees;  // Update current price in INR
        // Average price remains unchanged as it's historical
    }

    // Record sell transaction
    string transactionId = guuid();                    // Generate a unique transaction ID
    userData["stock_transactions"][transactionId] = {  // Record transaction details
        {"amount", saleProceeds},                       // Set transaction amount
        {"date", gettime()},                           // Set transaction date
        {"paymentMethod", "Wallet"},                   // Set payment method
        {"stock_name", stockid}                        // Set stock name
    };

    // Instead of currentUser .addMoneyToWallet(saleProceeds);
    double currentBalance = userData[getUser  ID()]["walletBalance"].get<double>(); // Get current wallet balance
    userData[getUser  ID()]["walletBalance"] = currentBalance + saleProceeds; // Update wallet balance

    ofstream outFile("test.json");                     // Open user data file for writing
    outFile << setw(4) << userData << endl;            // Write updated user data to the file
    outFile.close();                                    // Close the file

    cout << "Successfully sold " << sellQuantity << " shares of " << stockid << "\n"; // Confirm sale
    cout << "Sale proceeds: Rs." << saleProceeds << " added to wallet\n"; // Notify sale proceeds
    cout << "Updated wallet balance: Rs." << (currentBalance + saleProceeds) << "\n"; // Display updated balance
}

vector<StockMetrics> OwnedStock::calculateIndividualReturns() { // Function to calculate individual returns
    vector<StockMetrics> returns;                            // Vector to hold stock metrics
    json userData;                                          // JSON object to hold user data
    ifstream inFile("test.json");                           // Open user data file for reading
    if (!inFile.good()) {                                   // Check if file is accessible
        return returns;                                     // Return empty vector if file is not good
    }
    inFile >> userData;                                    // Read user data from the file
    inFile.close();                                        // Close the file

    auto portfolio = userData[getUser  ID()]["portfolio"]; // Get user's portfolio
    for (auto it = portfolio.begin(); it != portfolio.end(); ++it) { // Iterate through portfolio
        string stockName = it.key();                       // Get stock name
        if (stockName == "OwnedStockId") continue;       // Skip if stock name is "OwnedStockId"

        int quantity = it.value()["Quantity"].get<int>(); // Get quantity of stock
        double averagePrice = it.value()["AveragePrice"].get<double>(); // Get average price of stock
        
        string command = "java -cp lib/json-20240303.jar:. StockMetrics.java " + 
                        stockName + " " + 
                        to_string(quantity) + " " + 
                        to_string(averagePrice); // Command to calculate stock metrics
        
        //cout << "Executing command: " << command << endl; // Debugging output
        
        FILE* pipe = popen(command.c_str(), "r");        // Execute command and open pipe
        char buffer[1024];                                 // Buffer to hold command output
        string result = "";                                // String to accumulate output
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) { // Read output from pipe
            result += buffer;                              // Append output to result
        }
        int status = pclose(pipe);                         // Close pipe and get status
        
        //cout << "Java output: " << result << endl;      // Debugging output
        
        try {
            json metricsJson = json::parse(result);       // Parse JSON output
            StockMetrics metrics;                          // Create StockMetrics object
            metrics.stockName = stockName;                // Set stock name
            metrics.quantity = quantity;                   // Set quantity
            metrics.averageCost = averagePrice;            // Set average cost
            metrics.currentPrice = metricsJson["currentPrice"]; // Set current price
            metrics.totalInvested = metricsJson["totalInvested"]; // Set total invested
            metrics.currentValue = metricsJson["currentValue"]; // Set current value
            metrics.profitLoss = metricsJson["profitLoss"]; // Set profit/loss
            metrics.profitLossPercentage = metricsJson["profitLossPercentage"]; // Set profit/loss percentage
            metrics.dayChange = metricsJson["dayChange"];   // Set daily change
            metrics.dayChangePercentage = metricsJson["dayChangePercentage"]; // Set daily change percentage
            //metrics.profitLossType = metricsJson["profitLoss //metrics.profitLossType = metricsJson["profitLossType"]; // Set profit/loss type
            returns.push_back(metrics);                     // Add metrics to the returns vector
        } catch (const json::exception& e) {              // Catch JSON parsing exceptions
            cout << "JSON parsing error: " << e.what() << endl; // Print error message
            cout << "Raw output: " << result << endl;      // Print raw output for debugging
        }
    }

    return returns;                                       // Return the vector of stock metrics
}
