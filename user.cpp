#include<bits/stdc++.h>
#include<Stock.cpp>
#include<Transaction.cpp>
#include<portfolio.cpp>
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
    //Portfolio portfolio;
    vector<Stock> watching;
    vector<Transaction> transactionHistory;
    double walletBalance;
    Auth auth;
    bool isAuthenticated;

public:
    User(const string& userID, const string& name, const string& password, const Portfolio& portfolio) 
    : userID(userID), name(name), password(password), portfolio(portfolio), walletBalance(0.00), isAuthenticated(false) {}

    bool login(string userID, string password) {
        if (auth.login(userID, password)) {
            this->userID = userID;
            isAuthenticated = true;
            cout << "Logged in Successfully" << endl;
            return true;
        }
        cout << "Login failed. Please check your credentials." << endl;
        cout << "If you want to register, please register first." << endl;
        return false;
    }

    void logout() {
        auth.logout();
        userID.clear();
        isAuthenticated = false;
        cout << "Logged out successfully" << endl;
    }

    bool registerUser(string userID, string password) {
        if (auth.registerUser(userID, password)) {
            this->userID = userID;
            isAuthenticated = true;
            cout << "Registered successfully!" << endl;
            cout << "Welcome to our platform!" << endl;
            cout << "Start you stock trading journey today!" << endl;
            return true;
        }
        return false;
    }

    bool deleteAccount() {
        if (!isAuthenticated) {
            cout << "Must be logged in to delete account." << endl;
            return false;
        }
        if (auth.deleteAccount()) {
            userID.clear();
            isAuthenticated = false;
            cout << "Account deleted successfully!" << endl;
            return true;
        }
        cout << "Account deletion failed." << endl;
        return false;
    }

    string getUserID() const {
        return userID;
    }

    bool isLoggedIn() const {
        return isAuthenticated;
    }

    
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
