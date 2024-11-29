#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include<thread>
#include "Backend_Classes/Class_files/Header Files/stock.hpp"
#include "utils/utils.hpp"
#include <nlohmann/json.hpp>

// Color definitions
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

using json = nlohmann::json;
using namespace std::this_thread;
using namespace std::chrono;

Stock::Stock() : stockID(""),
    stockName(""),
    currentPrice(0.0),
    fiftyTwoWeekHigh(0.0),
    fiftyTwoWeekLow(0.0),
    userID("") {}

Stock::Stock(string stockID, string stockName, double currentPrice, string userId)
    : stockID(stockID),
      stockName(stockName),
      currentPrice(currentPrice),
      fiftyTwoWeekHigh(0.0),
      fiftyTwoWeekLow(0.0),
      userID(userId) {}

OwnedStock::OwnedStock() : Stock(),
    quantity(0),
    buyPrice(0.0) {}

OwnedStock::OwnedStock(string stockID, string stockName, double currentPrice,
    string userId, int quantity, double buyPrice)
    : Stock(stockID, stockName, currentPrice, userId),
      quantity(quantity),
      buyPrice(buyPrice) {}

void Stock::setUserID(const string& id) {
    userID = id;
}

string Stock::getUserID() {
    return userID;
}

string Stock::getstockname() const {
    return stockName;
}

string Stock::getstockname() {
    return stockName;
}

void Stock::setStockName(const string& name) {
    stockName = name;
}

double Stock::getPrice() const {
    return currentPrice;
}

void Stock::setCurrentPrice(double price) {
    currentPrice = price;
}

void Stock::displayDetails() {
    string command = "java -cp lib/json-20240303.jar searchapi.java " + stockID;
    system(command.c_str());
    // The c_str() converts the C++ string to a C-style string that system() can use.
    // command passed to the shell to execute by itself.
}

void Stock::buyStock(string stockName, int quantity) {
    json stockDetails;
    ifstream stockFile("Databases/stockdetails.json");
    if (!stockFile.good()) {
        throw runtime_error("Error reading stock details");
    }
    stockFile >> stockDetails;
    stockFile.close();

    setStockName(stockName);
    auto stockInfo = stockDetails[getstockname() + ":NASDAQ"]["price_info"];
    double currentPrice = stod(stockInfo["current_price"].get<string>());
    double totalCost = currentPrice * quantity;

    cout << BOLD << "\n=== 🛒 Stock Purchase Details 🛒 ===\n" << RESET;
    cout << "Loading current price";
    for(int i = 0; i < 3; i++) {
        sleep_for(milliseconds(300));
        cout << "." << flush;
    }
    cout << endl;
    
    cout << CYAN << "📈 Current Price: $" << currentPrice << RESET << endl;
    cout << BLUE << "📦 Quantity: " << quantity << RESET << endl;
    cout << GREEN << "💰 Total Cost: $" << totalCost << RESET << endl;
    cout << YELLOW << "\n🤔 Confirm purchase? (y/n): " << RESET;
    char choice;
    cin >> choice;

    if (choice == 'y') {
        cout << "\n🔄 Processing transaction";
        for(int i = 0; i < 4; i++) {
            sleep_for(milliseconds(300));
            cout << "." << flush;
        }
        cout << endl;
        User currentUser;
        currentUser.setUserID(userID);
        double walletBalance = currentUser.checkWalletBalance();
        
        if (walletBalance >= totalCost) {
            json userData;
            ifstream inFile("Databases/test.json");
            if (inFile.good()) {
                inFile >> userData;
            }
            inFile.close();

            // Get existing portfolio data
            int existingQuantity = 0;
            double existingAvgPrice = 0.0;
            
            if (userData[userID]["portfolio"].contains(stockName)) {
                existingQuantity = userData[userID]["portfolio"][stockName]["Quantity"].get<int>();
                existingAvgPrice = userData[userID]["portfolio"][stockName]["AveragePrice"].get<double>();
            }

            // Calculate new weighted average price
            double newAveragePrice = ((existingAvgPrice * existingQuantity) + (currentPrice * quantity)) / 
                                   (existingQuantity + quantity);

            // Update portfolio with new average and total quantity
            userData[userID]["portfolio"][stockName] = {
                {"AveragePrice", newAveragePrice},
                {"CurrentPrice", currentPrice},
                {"Quantity", existingQuantity + quantity}
            };

            string transactionId = guuid();
            userData["stock_transactions"][transactionId] = {
                {"amount", totalCost},
                {"date", gettime()},
                {"paymentMethod", "Wallet"},
                {"stock_name", stockName}
            };

            ofstream outFile("Databases/test.json");
            outFile << setw(4) << userData << endl;
            outFile.close();

            currentUser.removeMoneyfromWallet(totalCost);
            cout << "✅ Successfully purchased " << quantity << " shares of " << stockName << "\n";
        } else {
            cout << "❌ Insufficient funds. Current balance: $" << walletBalance << "\n";
        }
    }
}



void Stock::saveStockDataToLocal(string stockName, int quantity) {
    json userData;
    json stockDetails;
    string timestamp = gettime();
    string transactionId = guuid();

    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        inFile >> userData;
    }
    inFile.close();

    ifstream stockFile("Databases/stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
    }
    stockFile.close();

    double currentPrice = stod(stockDetails[stockName + ":NASDAQ"]["price_info"]["current_price"].get<string>());

    userData[userID]["portfolio"][stockName] = {
        {"AveragePrice", currentPrice},
        {"CurrentPrice", currentPrice},
        {"Quantity", quantity}
    };

    userData["stock_transactions"][transactionId] = {
        {"amount", currentPrice * quantity},
        {"date", timestamp},
        {"paymentMethod", "Wallet"},
        {"stock_name", stockName}
    };

    ofstream outFile("Databases/test.json");
    outFile << setw(4) << userData << endl;
    outFile.close();
}

void OwnedStock::sell(string stockid, int sellQuantity) {
    json userData;
    ifstream inFile("Databases/test.json");
    if (!inFile.good()) {
        throw runtime_error("Error reading transactions");
    }
    inFile >> userData;
    inFile.close();

    if (!userData[getUserID()]["portfolio"].contains(stockid)) {
        cout << "You don't own any shares of " << stockid << "\n";
        return;
    }

    int currentQuantity = userData[getUserID()]["portfolio"][stockid]["Quantity"].get<int>();
    if (currentQuantity < sellQuantity) {
        cout << "Insufficient shares. You own " << currentQuantity << " shares of " << stockid << "\n";
        return;
    }

    string command = "java -cp lib/json-20240303.jar currentprice.java " + stockid;
    system(command.c_str());

    json stockDetails;
    ifstream stockFile("Databases/stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
    }
    stockFile.close();

    double updatedPrice = stod(stockDetails[stockid + ":NASDAQ"]["price_info"]["current_price"].get<string>());
    double saleProceeds = sellQuantity * updatedPrice;



    cout << "\nStock Sale Details:\n";
    cout << BOLD << "\n=== 💹 Stock Sale Details 💹 ===\n" << RESET;
    cout << "Fetching current market price";
    for(int i = 0; i < 3; i++) {
        sleep_for(milliseconds(300));
        cout << "." << flush;
    }
    cout << endl;
    cout << CYAN << "📈 Current Price: $" << updatedPrice << RESET << endl;
    cout << BLUE << "📦 Quantity to Sell: " << sellQuantity << RESET << endl;
    cout << GREEN << "💰 Total Sale Value: $" << saleProceeds << RESET << endl;
    cout << YELLOW << "\n🤔 Confirm sale? (y/n): " << RESET;
    
    char choice;
    cin >> choice;
    if (choice != 'y') {
        cout << "Sale cancelled.\n";
        return;
    }
    cout << "\n🔄 Processing sale";
        for(int i = 0; i < 4; i++) {
            sleep_for(milliseconds(300));
            cout << "." << flush;
        }
    cout << endl;


    int remainingQuantity = currentQuantity - sellQuantity;
    if (remainingQuantity == 0) {
        userData[getUserID()]["portfolio"].erase(stockid);
    } else {
        userData[getUserID()]["portfolio"][stockid]["Quantity"] = remainingQuantity;
        userData[getUserID()]["portfolio"][stockid]["CurrentPrice"] = updatedPrice;
    }

    string transactionId = guuid();
    userData["stock_transactions"][transactionId] = {
        {"amount", saleProceeds},
        {"date", gettime()},
        {"paymentMethod", "Wallet"},
        {"stock_name", stockid}
    };

    double currentBalance = userData[getUserID()]["walletBalance"].get<double>();
    userData[getUserID()]["walletBalance"] = currentBalance + saleProceeds;

    ofstream outFile("Databases/test.json");
    outFile << setw(4) << userData << endl;
    outFile.close();

    cout << GREEN << "✅ Successfully sold " << sellQuantity << " shares of " << stockid << RESET << endl;
    cout << BLUE << "💵 Sale proceeds: $" << saleProceeds << " added to wallet" << RESET << endl;
    cout << CYAN << "🏦 Updated wallet balance: $" << (currentBalance + saleProceeds) << RESET << endl;
}

vector<StockMetrics> OwnedStock::calculateIndividualReturns() {
    vector<StockMetrics> returns;
    json userData;
    ifstream inFile("Databases/test.json");
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
        
        FILE* pipe = popen(command.c_str(), "r");
        char buffer[1024];
        string result = "";
        while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
            result += buffer;
        }
        pclose(pipe);
        
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
            metrics.dayChange = metricsJson["dayChange"];
            metrics.dayChangePercentage = metricsJson["dayChangePercentage"];
            returns.push_back(metrics);
        } catch (const json::exception& e) {
            cout << "JSON parsing error: " << e.what() << endl;
            cout << "Raw output: " << result << endl;
        }
    }
    return returns;
}

double OwnedStock::getReturns() const {
    double totalInvestment = buyPrice * quantity;
    double currentValue = getPrice() * quantity;
    return ((currentValue - totalInvestment) / totalInvestment) * 100;
}

double OwnedStock::getBuyPrice() const {
    return buyPrice;
}

int OwnedStock::getQuantity() const {
    return quantity;
}

void OwnedStock::printStockInfo() const {
    cout << BOLD << "\n=== 📊 Stock Information 📊 ===\n" << RESET;
    cout << BLUE << "🏢 Stock: " << getstockname() << RESET << endl;
    cout << "📦 Quantity: " << quantity << endl;
    cout << "💰 Buy Price: $" << fixed << setprecision(2) << buyPrice << endl;
    cout << "📈 Current Price: $" << getPrice() << endl;
    cout << "💸 Total Investment: $" << getTotalInvested() << endl;
    cout << "💎 Current Value: $" << (getPrice() * quantity) << endl;
    
    double returns = getReturns();
    if(returns >= 0) {
        cout << GREEN << "📈 Returns: +" << returns << "%" << RESET << endl;
    } else {
        cout << RED << "📉 Returns: " << returns << "%" << RESET << endl;
    }
}
double OwnedStock::getTotalInvested() const {
    return buyPrice * quantity;
}

double OwnedStock::getProfit() const {
    return (getPrice() * quantity) - (buyPrice * quantity);
}
