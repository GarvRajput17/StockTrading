#ifndef TRANSACTION_HPP
#define TRANSACTION_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using namespace std;
using json = nlohmann::json;

class Transaction {
private:
    string transactionID;
    string paymentMethod;
    double amount;
    string date;
    string type;
    string fileName = "test.json";
    static vector<Transaction> transactions;

public:
    Transaction(); // Add default constructor
    Transaction(string tID, string tType, double amount, string d, string pMtd);

    void displayTransaction();
    void recordTransaction(string userId, string transactionId, string type, double amount, string date, string paymentMethod);
    void loadTransactions(string userId);
};

#endif // TRANSACTION_HPP
