/*#include<bits/stdc++.h>
using namespace std;

class Portfolio;
class Stock;
class OwnedStock;
class Transaction;

class User {
private:
    string userID;
    string name;
    string password;
    Portfolio portfolio;
    vector<Stock> watching;
    vector<Transaction> transactionHistory;
    double walletBalance;

public:
    bool login(string userID, string password);
    void logout();
    bool registerUser(string userID, string password);
    bool deleteAccount();
    void addMoneyToWallet(double amount);
    void withdrawMoneyFromWallet(double amount);
    void viewTransactionHistory();
    void displayWatchlist();
    void addStockToWatchlist(Stock stock);
    void removeStockFromWatchlist(string stockID);

    // Portfolio operations
    void createPortfolio();
    void updatePortfolio(OwnedStock stock);
    void deletePortfolio();

    // Transaction history operations
    void addTransaction(Transaction transaction);
    void removeTransaction(Transaction transaction);

    // Helper Methods
    bool authenticate(string userID, string password);
    double checkWalletBalance();
};


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


class Transaction {
private:
    int transactionID;
    string stockID;
    string transactionType;
    int quantity;
    double transactionPrice;
    string date;

public:
    void displayTransaction();

    // Helper Methods
    void recordTransaction(User user);
};
*/
