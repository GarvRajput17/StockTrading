#include<bits/stdc++.h>
#include<Stock.cpp>
using namespace std;

class Portfolio {
private:
    int numberOfStocks;
    double totalInvested;
    double currentValue;
    double returns;
    vector<OwnedStock> holdings;

public:
    void addStock(OwnedStock stock);
    bool removeStock(string stockID);
    void displayPortfolio();
    void autoUpdate();
    void sortByProfit();
    void sortByLoss();
    void sortByAmountInvested();
    void sortByReturns();

    // Helper Methods
    void updateHoldings(OwnedStock stock);
    void adjustInvestmentValues(double changeAmount);
};