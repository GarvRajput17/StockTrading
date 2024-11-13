#ifndef USER_HPP
#define USER_HPP
#include <string>
#include <nlohmann/json.hpp>
#include "authentication/auth.hpp"

using json = nlohmann::json;
using std::string;

class User {
private:
    string userID;
    string name;
    string password;
    double walletBalance;
    Auth auth;
    bool isAuthenticated;

    void saveTransactionToLocal(double amount, const string& paymentMethod);
    void loadUserData();

public:
    // Constructors
    User();
    User(const string& userID, const string& name, const string& password);

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

    // Getters
    string getUserID() const;
    bool isLoggedIn() const;
};

#endif // USER_HPP
