#include <bits/stdc++.h>
#include <Stock.cpp>
using namespace std;

class Portfolio
{
private:
    int numberOfStocks;
    double totalPortfolioInvestment;
    double currentValue;
    double returns;
    vector<OwnedStock> holdings;
    unordered_map<string, OwnedStock> holdingsmap;
    string fileName = "transaction.json";
    string userID;

    public : void addStock(OwnedStock stock);
    bool removeStock(string stockID);
    void displayPortfolio() {
        int uniqueStocks = 0;
        double totalInvested = 0;
        double totalProfit = 0;
        double totalLoss = 0;
        double totalReturns = 0;

        for (int i = 0; i < holdings.size(); i++) {
            uniqueStocks++;
            totalInvested += holdings[i].totalInvested;
            totalProfit += holdings[i].getProfit();
            totalLoss += holdings[i].getLoss();
            totalReturns += holdings[i].getReturns();
        }

        cout << "Portfolio Details: " << endl;
        cout << "Number of Stocks: " << uniqueStocks << endl;
        cout << "Total Invested: " << totalInvested << endl;
        cout << "Total Profit: " << totalProfit << endl;
        cout << "Total Loss: " << totalLoss << endl;
        cout << "Total Returns: " << totalReturns << endl;
        cout << "Current Portfolio Value: " << currentValue << endl;
    }

    void calculateMetrics() {
        for (auto &stock : holdings) {
            // Recalculate metrics for each stock
            // Similarly, update based on quantity

            // These values will be updated for each stock
            double profit = stock.getProfit();
            double loss = stock.getLoss();
            double returns = stock.getReturns();

            // Store these in the stock object or update wherever needed
            stock.profit = profit;
            stock.loss = loss;
            stock.returns = returns;
        }
    }
    // void autoUpdate();
    // Sort portfolio by profit
    void sortByProfit() {
        sort(holdings.begin(), holdings.end(), [](const OwnedStock& a, const OwnedStock& b) {
            return a.getProfit() > b.getProfit();
        });
    }

    // Sort portfolio by loss
    void sortByLoss() {
        sort(holdings.begin(), holdings.end(), [](const OwnedStock& a, const OwnedStock& b) {
            return a.getLoss() > b.getLoss();
        });
    }

    // Sort portfolio by total invested
    void sortByAmountInvested() {
        sort(holdings.begin(), holdings.end(), [](const OwnedStock& a, const OwnedStock& b) {
            return a.totalInvested > b.totalInvested;
        });
    }

    // Sort portfolio by returns
    void sortByReturns() {
        sort(holdings.begin(), holdings.end(), [](const OwnedStock& a, const OwnedStock& b) {
            return a.getReturns() > b.getReturns();
        });
    }

    // Helper Methods
    // void updateHoldings(OwnedStock stock);
    // void adjustInvestmentValues(double changeAmount);

    Portfolio(int numberOfStocks, double totalInvested, double currentValue, double returns, vector<OwnedStock> holdings,string UserID)
    {
        this->numberOfStocks = numberOfStocks;
        this->totalInvested = totalInvested;
        this->currentValue = currentValue;
        this->returns = returns;
        this->holdings = holdings;
        this->userID = userID;
    }

    void loadPortfolio()
    {
        ifstream inFile(fileName);
        if (!inFile.is_open())
        {
            cerr << "Error opening file: " << fileName << endl;
            return;
        }

        json data;
        inFile >> data;
        inFile.close();

        if (data["users"].contains(userID) && data["users"][userID].contains("portfolio"))
        {
            json portfolioData = data["users"][userID]["portfolio"];

            for (auto &[stockID, stockData] : portfolioData)
            {
                double quantity = stockData["quantity"];
                double invested = stockData["totalInvested"];

                OwnedStock *temp = new OwnedStock(stockID, quantity, invested);
                holdings.push_back(&temp);
                holdingsmap[stockID] = temp;
                numberOfStocks++;
                totalPortfolioInvestment += temp.totalInvested;

            }
        }
    }

    void savePortfolio() {

        json data;
        for (const auto &[stockID, stock] : holdingsmap) {
            data["users"][userID]["portfolio"][stockID] = {
                {"quantity", stock.quantity},
                {"totalInvested", stock.totalInvested}
            };
        }

        std::ofstream outFile(filename);
        if (!outFile) {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            return;
        }

        outFile << jsonData.dump(4); // Pretty print with 4-space indentation
        outFile.close();
        }

};
