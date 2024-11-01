#include<bits/stdc++.h>
#include<user.cpp>
using namespace std;
class Stock {
private:
    string stockID;
    string stockName;
    double currentPrice;
    double fiftyTwoWeekHigh;
    double fiftyTwoWeekLow;

public:
    void buyStock(int quantity, double price);
    void addToWatchlist(User user);
    double calculateVolatility();
    void displayGraph();
    void displayDetails();

    // Update Methods
    void updatePrice(double newPrice);
    void update52WeekHigh(double newHigh);
    void update52WeekLow(double newLow);
};


class OwnedStock : public Stock {
private:
    int quantity;
    double buyPrice;
    double returns;

public:
    void sell(int quantity, double price);
    double calculateIndividualReturns();
    void updateQuantity(int newQuantity);

    // Helper Methods
    void updateReturns(double newPrice);
};