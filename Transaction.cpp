#include<bits/stdc++.h>
#include<user.cpp>
using namespace std;

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
