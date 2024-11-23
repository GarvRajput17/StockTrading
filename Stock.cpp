#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "stock.hpp"
#include <utils/utils.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

Stock::Stock() : user() {}

void Stock::setUserID(const string& id) {
    userID = id;
}
string Stock::getUserID() {
    return userID;
}


string Stock::getstockname() {
    return stockName;
}

void Stock::setStockName(const string& name) {
    stockName = name;
}

void Stock::displayDetails() {
    string command = "java -cp lib/json-20240303.jar searchapi.java ";
    system(command.c_str());
}

void Stock::buyStock(string stockName, int quantity) {
    json stockDetails;
    ifstream stockFile("stockdetails.json");
    if (!stockFile.good()) {
        throw runtime_error("Error reading stock details");
    }
    stockFile >> stockDetails;
    stockFile.close();

    setStockName(stockName);
    auto stockInfo = stockDetails[getstockname() + ":NASDAQ"]["price_info"];
    double currentPrice = stod(stockInfo["current_price"].get<string>());
    const double USD_TO_INR = 83.0;
    double priceInRupees = currentPrice * USD_TO_INR;
    double totalPriceInRupees = priceInRupees * quantity;

    cout << "Stock Purchase Details:\n";
    cout << "Current Price: " << currentPrice << " USD (Rs." << priceInRupees << ")\n";
    cout << "Quantity: " << quantity << "\n";
    cout << "Total Cost: Rs." << totalPriceInRupees << "\n";
    cout << "Confirm purchase? (y/n): ";
    char choice;
    cin >> choice;

    if (choice == 'y') {
        User currentUser;
        currentUser.setUserID(userID);
        double walletBalance = currentUser.checkWalletBalance();
        
        if (walletBalance >= totalPriceInRupees) {
            json userData;
            ifstream inFile("test.json");
            if (inFile.good()) {
                inFile >> userData;
            }
            inFile.close();

            // Update portfolio
            userData[userID]["portfolio"][stockName] = {
                {"AveragePrice", currentPrice},
                {"CurrentPrice", currentPrice},
                {"Quantity", quantity}
            };

            // Record in stock_transactions section
            string transactionId = guuid();
            string timestamp = gettime();
            userData["stock_transactions"][transactionId] = {
                {"amount", totalPriceInRupees},
                {"date", timestamp},
                {"paymentMethod", "Wallet"},
                {"stock_name", stockName}
            };

            ofstream outFile("test.json");
            outFile << setw(4) << userData << endl;
            outFile.close();

            currentUser.removeMoneyfromWallet(totalPriceInRupees);

            cout << "Successfully purchased " << quantity << " shares of " << stockName << "\n";
            //cout << "Your updated wallet balance is Rs." << currentUser.checkWalletBalance() << "\n";
        } else {
            cout << "Insufficient funds. Current balance: Rs." << walletBalance << "\n";
        }
    }
}


void Stock::saveStockDataToLocal(string stockName, int quantity) {
    json userData;
    json stockDetails;
    string timestamp = gettime();
    string transactionId = guuid();

    // Load user data
    ifstream inFile("test.json");
    if (inFile.good()) {
        inFile >> userData;
    }
    inFile.close();

    // Load stock details and get current price
    ifstream stockFile("stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
    }
    stockFile.close();

    double currentPrice = stod(stockDetails[stockName + ":NASDAQ"]["price_info"]["current_price"].get<string>());
    const double USD_TO_INR = 83.0;
    double priceInRupees = currentPrice * USD_TO_INR;

    // Update portfolio
    // Update portfolio
    userData[userID]["portfolio"][stockName] = {
        {"AveragePrice", priceInRupees * quantity / quantity},  // Average price in INR
        {"CurrentPrice", priceInRupees},  // Current price in INR
        {"Quantity", quantity}
    };


    // Record in stock_transactions
    userData["stock_transactions"][transactionId] = {
        {"amount", priceInRupees * quantity},
        {"date", timestamp},
        {"paymentMethod", "Wallet"},
        {"stock_name", stockName}
    };

    ofstream outFile("test.json");
    outFile << setw(4) << userData << endl;
    outFile.close();
}

void OwnedStock::sell(string stockid, int sellQuantity) {
    json userData;
    ifstream inFile("test.json");
    if (!inFile.good()) {
        throw runtime_error("Error reading transactions");
    }
    inFile >> userData;
    inFile.close();

    // Check if user owns the stock and has sufficient quantity
    if (!userData[getUserID()]["portfolio"].contains(stockid)) {
        cout << "You don't own any shares of " << stockid << "\n";
        return;
    }

    int currentQuantity = userData[getUserID()]["portfolio"][stockid]["Quantity"].get<int>();
    if (currentQuantity < sellQuantity) {
        cout << "Insufficient shares. You own " << currentQuantity << " shares of " << stockid << "\n";
        return;
    }

    // Get current price
    string command = "java -cp lib/json-20240303.jar currentprice.java " + stockid;
    system(command.c_str());

    json stockDetails;
    ifstream stockFile("stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
    }
    stockFile.close();

    double updatedPrice = stod(stockDetails[stockid + ":NASDAQ"]["price_info"]["current_price"].get<string>());
    const double USD_TO_INR = 83.0;
    double priceInRupees = updatedPrice * USD_TO_INR;
    double saleProceeds = sellQuantity * priceInRupees;

    // Show sale details and get confirmation
    cout << "\nStock Sale Details:\n";
    cout << "Current Price: " << updatedPrice << " USD (Rs." << priceInRupees << ")\n";
    cout << "Quantity to Sell: " << sellQuantity << "\n";
    cout << "Total Sale Value: Rs." << saleProceeds << "\n";
    cout << "Confirm sale? (y/n): ";
    
    char choice;
    cin >> choice;
    if (choice != 'y') {
        cout << "Sale cancelled.\n";
        return;
    }

    User currentUser;
    currentUser.setUserID(getUserID());

    // Update portfolio quantity
    int remainingQuantity = currentQuantity - sellQuantity;
    if (remainingQuantity == 0) {
        // Remove stock entry completely if quantity becomes 0
        userData[getUserID()]["portfolio"].erase(stockid);
    } else {
        userData[getUserID()]["portfolio"][stockid]["Quantity"] = remainingQuantity;
        userData[getUserID()]["portfolio"][stockid]["CurrentPrice"] = priceInRupees;  // Store in INR
        // Average price remains unchanged as it's historical
    }

    // Record sell transaction
    string transactionId = guuid();
    userData["stock_transactions"][transactionId] = {
        {"amount", saleProceeds},
        {"date", gettime()},
        {"paymentMethod", "Wallet"},
        {"stock_name", stockid}
    };

    // Instead of currentUser.addMoneyToWallet(saleProceeds);
double currentBalance = userData[getUserID()]["walletBalance"].get<double>();
userData[getUserID()]["walletBalance"] = currentBalance + saleProceeds;

ofstream outFile("test.json");
outFile << setw(4) << userData << endl;
outFile.close();

cout << "Successfully sold " << sellQuantity << " shares of " << stockid << "\n";
cout << "Sale proceeds: Rs." << saleProceeds << " added to wallet\n";
cout << "Updated wallet balance: Rs." << (currentBalance + saleProceeds) << "\n";

}




vector<StockMetrics> OwnedStock::calculateIndividualReturns() {
    vector<StockMetrics> returns;
    json userData;
    ifstream inFile("test.json");
    if (!inFile.good()) {
        return returns;
    }
    inFile >> userData;
    inFile.close();

    auto portfolio = userData[getUserID()]["portfolio"];
    for (auto it = portfolio.begin(); it != portfolio.end(); ++it) {
        string stockName = it.key();
        if (stockName == "OwnedStockId") continue;

        int quantity = it.value()["Quantity"].get<int>();
        double averagePrice = it.value()["AveragePrice"].get<double>();
        
        string command = "java -cp lib/json-20240303.jar:. StockMetrics.java " + 
                        stockName + " " + 
                        to_string(quantity) + " " + 
                        to_string(averagePrice);
        
        //cout << "Executing command: " << command << endl;
        
        FILE* pipe = popen(command.c_str(), "r");
        char buffer[1024];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
        int status = pclose(pipe);
        
        //cout << "Java output: " << result << endl;
        
        try {
            json metricsJson = json::parse(result);
            StockMetrics metrics;
            metrics.stockName = stockName;
            metrics.quantity = quantity;
            metrics.averageCost = averagePrice;
            metrics.currentPrice = metricsJson["currentPrice"];
            metrics.totalInvested = metricsJson["totalInvested"];
            metrics.currentValue = metricsJson["currentValue"];
            metrics.profitLoss = metricsJson["profitLoss"];
            metrics.profitLossPercentage = metricsJson["profitLossPercentage"];
            //metrics.profitLossType = metricsJson["profitLossType"];
            returns.push_back(metrics);
        } catch (const json::exception& e) {
            cout << "JSON parsing error: " << e.what() << endl;
            cout << "Raw output: " << result << endl;
        }
    }

    return returns;
}
