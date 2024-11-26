#include "transaction.hpp"                       // Include the header file for the Transaction class
#include <iostream>                              // Include the standard input-output stream library
#include <fstream>                               // Include the file stream library for file operations
#include <iomanip>                               // Include the library for manipulating output formats
// We create a static vector to store the transactions
vector<Transaction> Transaction::transactions;  // Static vector to hold all Transaction objects

// Default constructor for the Transaction class
Transaction::Transaction()
    : transactionID(""),                         // Initialize transactionID to an empty string
      type(""),                                  // Initialize type to an empty string
      amount(0.0),                               // Initialize amount to 0.0
      date(""),                                  // Initialize date to an empty string
      paymentMethod("") {}                        // Initialize paymentMethod to an empty string

// Parameterized constructor for the Transaction class
Transaction::Transaction(string tID, string tType, double amount, string d, string pMtd)
    : transactionID(tID),                        // Initialize transactionID with provided tID
      type(tType),                               // Initialize type with provided tType
      amount(amount),                            // Initialize amount with provided amount
      date(d),                                   // Initialize date with provided d
      paymentMethod(pMtd) {}                     // Initialize paymentMethod with provided pMtd

// Function to display all the transactions 
void Transaction::displayTransaction() {
    // Choose the type of transactions that you want to view
    for (auto x : transactions) {                // Iterate through each transaction in the static vector
        cout << left  << "\n"                     // Output formatting for left alignment
             <<  "Date: " << x.date << "\n"     // Display the date of the transaction
             <<   fixed << setprecision(2) << "Amount: " <<  x.amount << "\n" // Display the amount with 2 decimal precision
             <<   "Type of Transaction: " << x.type << "\n" // Display the type of transaction
             << "Payment Method: " <<  x.paymentMethod << endl; // Display the payment method
    }
    cout << string(60, '-') << endl;             // Print a separator line
}

// Function to record a transaction and storing it in the json
void Transaction::recordTransaction(string userId, string transactionId, string type, double amount, string date, string paymentMethod) {
    ifstream inFile(fileName);                    // Open the JSON file for reading
    json data;                                    // Create a JSON object to hold data
    inFile >> data;                               // Read data from the file
    inFile.close();                               // Close the file

    json transactionData = {                      // Create a JSON object for the transaction
        {"amount", amount},                       // Add amount to the transaction data
        {"date", date},                           // Add date to the transaction data
        {"paymentMethod", paymentMethod},         // Add payment method to the transaction data
        {"type", type}                            // Add type to the transaction data
    };

    data[userId]["transactions"][transactionId] = transactionData; // Store the transaction data under the user ID

    ofstream outFile(fileName);                   // Open the JSON file for writing
    outFile << setw(4) << data << endl;          // Write the updated data to the file with pretty formatting
    outFile.close();                              // Close the file
}

// Function to load transactions from the json file
void Transaction::loadTransactions(string userId) {
    transactions.clear();                         // Clear the existing transactions in the static vector
    ifstream inFile(fileName);                    // Open the JSON file for reading
    if (!inFile.is_open()) {                      // Check if the file was opened successfully
        return;                                   // Exit the function if the file could not be opened
    }

    json data;                                    // Create a JSON object to hold data
    inFile >> data;                               // Read data from the file
    inFile.close();                               // Close the file

    // Match the exact JSON structure
    if (data.contains(userId) && data[userId].contains("transactions")) { // Check if user ID exists and has transactions
        auto userTransactions = data[userId]["transactions"]; // Get the user's transactions
        for (const auto& [transactionId, transactionData] : userTransactions.items()) { // Iterate through each transaction
            Transaction transaction(               // Create a Transaction object with the transaction data
                transactionId,                    // Use transactionId from the JSON
                transactionData["type"],         // Use type from the JSON
                transactionData["amount"],       // Use amount from the JSON
                transactionData["date"],         // Use date from the JSON
                transactionData["paymentMethod"] // Use payment method from the JSON
            );
            transactions.push_back(transaction);  // Add the transaction to the static vector
        }
    }
}
