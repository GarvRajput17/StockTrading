#include "transaction.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
// We create a static vector to store the transactions
vector<Transaction> Transaction::transactions;
// Default constructor fotr the Transaction class
Transaction::Transaction()
    : transactionID(""), type(""), amount(0.0), date(""), paymentMethod("") {}
// Parameterized constructor for the Transaction class
Transaction::Transaction(string tID, string tType, double amount, string d, string pMtd)
    : transactionID(tID), type(tType), amount(amount), date(d), paymentMethod(pMtd) {}

// Function to display all the transactions 
void Transaction::displayTransaction() {
// Choose the type of transactions that you want to view
    for (auto x : transactions) {
        cout << left  << "\n"
             <<  "Date: " << x.date << "\n"
             <<   fixed << setprecision(2) << "Amount: " <<  x.amount << "\n"
             <<   "Type of Transaction: " << x.type << "\n"
             << "Payment Method: " <<  x.paymentMethod << endl;
    }
    cout << string(60, '-') << endl;
}

// Function to record a transaction and stroing it in the json
void Transaction::recordTransaction(string userId, string transactionId, string type, double amount, string date, string paymentMethod) {
    ifstream inFile(fileName);
    json data;
    inFile >> data;
    inFile.close();

    json transactionData = {
        {"amount", amount},
        {"date", date},
        {"paymentMethod", paymentMethod},
        {"type", type}
    };

    data[userId]["transactions"][transactionId] = transactionData;

    ofstream outFile(fileName);
    outFile << setw(4) << data << endl;
    outFile.close();
}
// Function to load transactions from the json file
void Transaction::loadTransactions(string userId) {
    transactions.clear();
    ifstream inFile(fileName);
    if (!inFile.is_open()) {
        return;
    }

    json data;
    inFile >> data;
    inFile.close();

    // Match the exact JSON structure
    if (data.contains(userId) && data[userId].contains("transactions")) {
        auto userTransactions = data[userId]["transactions"];
        for (const auto& [transactionId, transactionData] : userTransactions.items()) {
            Transaction transaction(
                transactionId,
                transactionData["type"],
                transactionData["amount"],
                transactionData["date"],
                transactionData["paymentMethod"]
            );
            transactions.push_back(transaction);
        }
    }
}


