#include<bits/stdc++.h>
#include<jni.h>

using namespace std;

class StockInfo {
private:
    string stockSymbol, stockPrice, priceChange, percentageChange;
    string movement; // wether the stock price has gone up or has gone down
    
public:
    void setStockSymbol(const string& symbol) {
        stockSymbol = symbol;
    }
    string getStockSymbol() const {
        return stockSymbol;
    }
    void fetchStockData() {
        
    }






};

class Realtime {
    // for stock updates, daily high low, current price etc
};

class CompanyStats {
    // revenue, earnings, profit
};

class Predictions : public Realtime , public CompanyStats {
    // buy / sell predictions by expert opinions

};

class User {
    
};

class Admin : public User {

};

class Transcation {

};

class Buy : public Transcation {

};

class Sell : public Transcation {

};

class Portfolio {


};

class Order {

};

class Marketorder : public Order {

};

class Limitorder : public Order {

};



