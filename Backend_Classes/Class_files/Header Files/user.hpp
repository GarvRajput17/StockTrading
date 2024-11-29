#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <nlohmann/json.hpp>
#include "authentication/auth.hpp"
#include "Backend_Classes/Class_files/Header Files/Transaction.hpp" // Add this for Transaction class
#include "Backend_Classes/Class_files/Header Files/stock.hpp"  // Add this for Stock class

using json = nlohmann::json; // json object
using std::string; // for string 
class Stock; // forward declaration for avioding Circular dependencies
class User {
private:
    string userID;
    string name;
    string password;
    double walletBalance;
    Auth auth;
    bool isAuthenticated;
    Transaction transactionManager;

    void saveTransactionToLocal(double amount, const string& paymentMethod);
    void loadUserData();

public:
    // Constructors
    User();
    User(const string& userID, const string& name, const string& password);
    void setUserID(const string& id);

    // Authentication methods
    bool login(string userID, string password);
    void logout();
    bool registerUser(string userID, string password);
    bool deleteAccount();

    // Wallet operations
    void processPayment(double amount);
    void addMoneyToWallet(double amount);
    void removeMoneyfromWallet(double amount);
    double checkWalletBalance();

    // Watchlist operations
    void displayWatchlist();
    void addToWatchlist(Stock stock);
    void removeFromWatchlist(string stockId);

    // Getters
    string getUserID() const;
    bool isLoggedIn() const;

    // Transaction methods
    void displayTransactionHistory();
};

#endif // USER_HPP
