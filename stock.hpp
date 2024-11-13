#ifndef STOCK_HPP
#define STOCK_HPP

#include <string>
#include "user.hpp"
#include <nlohmann/json.hpp>

using std::string;

class Stock {
private:
    string stockID;
    string stockName;
    double currentPrice;
    double fiftyTwoWeekHigh;
    double fiftyTwoWeekLow;

public:
    User user;
    Stock();
    string getstockname();
    void saveStockDataToLocal(string stockName, int quantity);
    void displayDetails();
    void buyStock(string stockName, int quantity);
};

class OwnedStock : public Stock {
private:
    int quantity;
    double buyPrice;
    double returns;

public:
    void sell(int sellQuantity, double currentPrice);
    double calculateIndividualReturns();
    void updateQuantity(int newQuantity);
    void updateReturns(double currentPrice);
};

#endif // STOCK_HPP
