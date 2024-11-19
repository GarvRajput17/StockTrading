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
            currentUser.removeMoneyfromWallet(totalPriceInRupees);
            saveStockDataToLocal(stockName, quantity);
            cout << "Successfully purchased " << quantity << " shares of " << stockName << "\n";
            cout << "Your updated wallet balance is Rs." << currentUser.checkWalletBalance() << "\n";
        } else {
            cout << "Insufficient funds. Current balance: Rs." << walletBalance << "\n";
        }
    }
}

void Stock::saveStockDataToLocal(string stockName, int quantity) {
    json stockData;
    json stockDetails;
    string buytime = gettime();
    string transactionid = guuid();

    // Load transactions data
    ifstream transFile("transactions.json");
    if (transFile.good()) {
        transFile >> stockData;
    }
    transFile.close();

    // Load stock details and get current price
    ifstream stockFile("stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
    }
    stockFile.close();

    double currentPrice = stod(stockDetails[stockName + ":NASDAQ"]["price_info"]["current_price"].get<string>());

    // Create new stock purchase entry
    json newStockPurchase = {
        {"stockName", stockName},
        {"quantity", quantity},
        {"buyTime", buytime},
        {"transactionId", transactionid},
        {"buyPrice", currentPrice},
        {"currency", "USD"}
    };

    // Update the user's StocksOwned section
    stockData["users"][getUserID()]["StocksOwned"][stockName] = newStockPurchase;

    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();
}


void OwnedStock::sell(int sellQuantity, double currentPrice) {
    json stockData;
    ifstream transFile("transactions.json");
    if (!transFile.good()) {
        throw runtime_error("Error reading transactions");
    }
    transFile >> stockData;
    transFile.close();

    const double USD_TO_INR = 83.0;
    double priceInRupees = currentPrice * USD_TO_INR;
    double saleProceeds = sellQuantity * priceInRupees;

    User currentUser;
    currentUser.setUserID(getUserID());  // Use the getUserID() method


    json sellTransaction = {
        {"type", "SELL"},
        {"quantity", sellQuantity},
        {"sellPrice", currentPrice},
        {"sellPriceINR", priceInRupees},
        {"sellTime", gettime()},
        {"transactionId", guuid()},
        {"saleProceeds", saleProceeds}
    };

    

    stockData["users"][getUserID()]["StocksOwned"][getstockname()]["transactions"].push_back(sellTransaction);
    updateQuantity(quantity - sellQuantity);
    updateReturns(currentPrice);
    currentUser.addMoneyToWallet(saleProceeds);

    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();

    cout << "Successfully sold " << sellQuantity << " shares of " << getstockname() << "\n";
    cout << "Sale proceeds: Rs." << saleProceeds << " added to wallet\n";
}

double OwnedStock::calculateIndividualReturns() {
    json stockData;
    double totalReturns = 0.0;
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> stockData;
    }
    inFile.close();

    auto userStocks = stockData["users"][getUserID()]["StocksOwned"][getstockname()]["transactions"];
    for (const auto& transaction : userStocks) {
        if (transaction["type"] == "SELL") {
            double buyPrice = transaction["buyPrice"];
            double sellPrice = transaction["sellPrice"];
            int quantity = transaction["quantity"];
            totalReturns += (sellPrice - buyPrice) * quantity;
        }
    }
    return totalReturns;
}

void OwnedStock::updateQuantity(int newQuantity) {
    quantity = newQuantity;
    json stockData;
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> stockData;
    }
    inFile.close();

    stockData["users"][getUserID()]["StocksOwned"][getstockname()]["currentQuantity"] = quantity;
    
    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();
}

void OwnedStock::updateReturns(double currentPrice) {
    json stockData;
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> stockData;
    }
    inFile.close();

    auto& userStocks = stockData["users"][getUserID()]["StocksOwned"][getstockname()];
    for (auto& transaction : userStocks["transactions"]) {
        if (transaction["type"] != "SELL") {
            double buyPrice = transaction["buyPrice"];
            int quantity = transaction["quantity"];
            double transactionReturn = (currentPrice - buyPrice) * quantity;
            transaction["currentReturn"] = transactionReturn;
        }
    }

    returns = calculateIndividualReturns();
    userStocks["totalReturns"] = returns;
    
    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();
}
