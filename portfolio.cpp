#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "Stock.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Portfolio {
private:
    int numberOfStocks;
    double totalPortfolioInvestment;
    double currentValue;
    double returns;
    vector<OwnedStock> holdings;
    unordered_map<string, OwnedStock> holdingsMap;
    const string fileName = "test.json";
    string userID;

public:
    Portfolio(int numberOfStocks, double totalInvested, double currentValue, 
             double returns, vector<OwnedStock> holdings, string userID)
        : numberOfStocks(numberOfStocks),
          totalPortfolioInvestment(totalInvested),
          currentValue(currentValue),
          returns(returns),
          holdings(holdings),
          userID(userID) {}

    void displayPortfolio() {
        int uniqueStocks = 0;
        double totalInvested = 0.0;
        double totalProfit = 0.0;
        double totalReturns = 0.0;
        double currentPortfolioValue = 0.0;

        cout << "\n=== Portfolio Summary ===\n";
        cout << "----------------------------------------\n";

        for (const auto& stock : holdings) {
            uniqueStocks++;
            double investment = stock.getBuyPrice() * stock.getQuantity();
            double currentStockValue = stock.getPrice() * stock.getQuantity();
            
            totalInvested += investment;
            totalProfit += (currentStockValue - investment);
            currentPortfolioValue += currentStockValue;

            stock.printStockInfo();
        }

        totalReturns = (totalInvested > 0) ? (totalProfit / totalInvested) * 100 : 0;

        cout << "\n=== Overall Portfolio Metrics ===\n";
        cout << "Number of Unique Stocks: " << uniqueStocks << "\n";
        cout << "Total Investment: ₹" << fixed << setprecision(2) << totalInvested << "\n";
        cout << "Current Portfolio Value: ₹" << currentPortfolioValue << "\n";
        cout << "Total Profit/Loss: ₹" << totalProfit << "\n";
        cout << "Overall Returns: " << totalReturns << "%\n";
        cout << "----------------------------------------\n";
    }

    void loadPortfolio() {
        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            throw runtime_error("Unable to open " + fileName);
        }

        json data;
        inFile >> data;
        inFile.close();

        if (!data.contains(userID) || !data[userID].contains("portfolio")) {
            return;
        }

        json portfolioData = data[userID]["portfolio"];
        
        for (const auto& [stockID, stockData] : portfolioData.items()) {
            if (stockID == "OwnedStockId") continue;

            int quantity = stockData["Quantity"].get<int>();
            double averagePrice = stockData["AveragePrice"].get<double>();
            double currentPrice = stockData["CurrentPrice"].get<double>();

            OwnedStock stock(
                stockID,                    // stockID
                stockID,                    // using stockID as name temporarily
                currentPrice,               // currentPrice
                userID,                     // userID
                quantity,                   // quantity
                averagePrice               // buyPrice
            );

            holdings.push_back(stock);
            holdingsMap[stockID] = stock;
            
            numberOfStocks++;
            totalPortfolioInvestment += (averagePrice * quantity);
        }
    }

    void savePortfolio() {
        json data;
        ifstream inFile(fileName);
        if (inFile.good()) {
            inFile >> data;
        }
        inFile.close();

        for (const auto& [stockID, stock] : holdingsMap) {
            data[userID]["portfolio"][stockID] = {
                {"Quantity", stock.getQuantity()},
                {"AveragePrice", stock.getBuyPrice()},
                {"CurrentPrice", stock.getPrice()}
            };
        }

        ofstream outFile(fileName);
        outFile << setw(4) << data << endl;
        outFile.close();
    }

    void sortByProfit() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getProfit() > b.getProfit();
             });
    }

    void sortByLoss() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getProfit() < b.getProfit();
             });
    }

    void sortByAmountInvested() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getTotalInvested() > b.getTotalInvested();
             });
    }

    void sortByReturns() {
        sort(holdings.begin(), holdings.end(), 
             [](const OwnedStock& a, const OwnedStock& b) {
                 return a.getReturns() > b.getReturns();
             });
    }
};
