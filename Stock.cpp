#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include "stock.hpp"
#include<utils/utils.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

// Stock class implementations
Stock::Stock() : user() {}

string Stock::getstockname() {
    return stockName;
}

void Stock::saveStockDataToLocal(string stockName, int quantity) {
    json stockData;
    json stockDetails;
    string buytime = gettime();
    string transactionid = guuid();

    ifstream transFile("transactions.json");
    if (transFile.good()) {
        transFile >> stockData;
        transFile.close();
    }

    ifstream stockFile("stockdetails.json");
    if (stockFile.good()) {
        stockFile >> stockDetails;
        stockFile.close();
    }

    auto stockInfo = stockDetails[stockName + ":NASDAQ"]["summary"];
    double currentPrice = stockInfo["price"];
    
    json newStockPurchase = {
        {"stockName", stockName},
        {"quantity", quantity},
        {"buyTime", buytime},
        {"transactionId", transactionid},
        {"buyPrice", currentPrice},
        {"priceChange", {
            {"amount", stockInfo["price_change"]["amount"]},
            {"percentage", stockInfo["price_change"]["percentage"]},
            {"movement", stockInfo["price_change"]["movement"]}
        }},
        {"exchange", stockInfo["exchange"]},
        {"currency", stockInfo["currency"]}
    };

    stockData["users"]["userID"]["StocksOwned"][stockName] = newStockPurchase;

    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();
}

void Stock::displayDetails() {
    string command = "java -cp lib/json-20240303.jar searchapi.java ";
    system(command.c_str());
}

void Stock::buyStock(string stockName, int quantity) {
    json userdata;
    ifstream inFile("stockdetails.json");
    if (inFile.good()) {
        inFile >> userdata;
    }
    else {
        throw runtime_error("Error reading file");
    }
    inFile.close();

    auto stockinfo = userdata[stockName + ":NASDAQ"];
    auto summary = stockinfo["summary"];
    auto price = stockinfo["price"];
    auto price_value = price.get<double>();
    int totalprice = price_value * quantity;

    cout << "The total price of the stock you want to buy is:" << totalprice << endl;
    cout << "Do you want to buy the stock? (y/n)" << endl;
    char choice;
    cin >> choice;
    
    if (choice == 'y') {
        if (user.checkWalletBalance() < totalprice) {
            cout << "You don't have enough balance to buy the stock." << endl;
            cout << "Do you want to add money to your wallet? (y/n)" << endl;
            char choice1;
            cin >> choice1;
            if (choice1 == 'y') {
                cout << "Enter the amount you want to add to your wallet: ";
                double amount;
                cin >> amount;
                user.addMoneyToWallet(amount);
                cout << "Money added to your wallet." << endl;
            }
            else {
                cout << "You don't have enough balance to buy the stock." << endl;
                return;
            }
        }
        else {
            user.removeMoneyfromWallet(totalprice);
            cout << "Stock" << stockName << "with quantity" << quantity << "bought successfully." << endl;
        }
    }
}

// OwnedStock class implementations
void OwnedStock::sell(int sellQuantity, double currentPrice) {
    json stockData;
    string sellTime = gettime();
    string transactionId = guuid();
    
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> stockData;
    }
    inFile.close();

    double saleProceeds = sellQuantity * currentPrice;
    user.addMoneyToWallet(saleProceeds);

    json sellTransaction = {
        {"type", "SELL"},
        {"quantity", sellQuantity},
        {"sellPrice", currentPrice},
        {"sellTime", sellTime},
        {"transactionId", transactionId},
        {"saleProceeds", saleProceeds}
    };

    stockData["users"][user.getUserID()]["StocksOwned"][getstockname()]["transactions"].push_back(sellTransaction);
    updateQuantity(quantity - sellQuantity);
    updateReturns(currentPrice);
    
    ofstream outFile("transactions.json");
    outFile << setw(4) << stockData << endl;
    outFile.close();
}

double OwnedStock::calculateIndividualReturns() {
    json stockData;
    double totalReturns = 0.0;
    
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> stockData;
    }
    inFile.close();

    auto userStocks = stockData["users"][user.getUserID()]["StocksOwned"][getstockname()]["transactions"];
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
    
    stockData["users"][user.getUserID()]["StocksOwned"][getstockname()]["currentQuantity"] = quantity;
    
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

    auto& userStocks = stockData["users"][user.getUserID()]["StocksOwned"][getstockname()];
    
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
