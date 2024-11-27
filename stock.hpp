#ifndef STOCK_HPP // check if stock.hpp is not defined
#define STOCK_HPP // if not defined, define it

#include <string>
#include "user.hpp"
#include <nlohmann/json.hpp>

using std::string;
class User;
struct StockMetrics {
    string stockName;
    int quantity;
    double averageCost;
    double currentPrice;
    double totalInvested;
    double currentValue;
    double profitLoss;
    double profitLossPercentage;
    double dayChange;
    double dayChangePercentage;
    string movement;
};

class Stock {
private:
    std::string stockID;
    std::string stockName;
    double currentPrice;
    double fiftyTwoWeekHigh;
    double fiftyTwoWeekLow;
    std::string userID;

public:
    //User user;
    Stock();
    Stock(std::string stockID, std::string stockName, double currentPrice, std::string userId);
    std::string getstockname() const;
    string getstockname();
    void setUserID(const std::string& id);
    std::string getUserID();
    void setStockName(const std::string& name);
    double getPrice() const;
    void setCurrentPrice(double price);
    void saveStockDataToLocal(std::string stockName, int quantity);
    void displayDetails();
    void buyStock(std::string stockName, int quantity);

};  

class OwnedStock : public Stock {
private:
    //string stockID;
    int quantity;
    double buyPrice;

public:
    OwnedStock();

    OwnedStock(std::string stockID, std::string stockName,double currentPrice, std::string userId, int quantity, double buyPrice);
    int getQuantity() const;
    void updateQuantity(int newQuantity);
    double getBuyPrice() const;
    void updateBuyPrice(double newPrice);
    void sell(string stocknamee, int sellQuantity);
    vector<StockMetrics> calculateIndividualReturns();
    void updateReturns(double currentPrice);
    void printStockInfo() const;
    double getProfit() const ;
    double getTotalInvested() const;
    double getReturns() const;
};

#endif // STOCK_HPP
