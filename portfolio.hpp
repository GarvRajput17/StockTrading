#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <vector>
#include <unordered_map>
#include "Stock.hpp"

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
             double returns, vector<OwnedStock> holdings, string userID);
             
    void displayPortfolio();
    void loadPortfolio();
    void savePortfolio();
    void sortByProfit();
    void sortByLoss();
    void sortByAmountInvested();
    void sortByReturns();
};

#endif
