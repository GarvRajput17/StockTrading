#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include <thread>
#include <jni.h>
#include "user.hpp"
#include "stock.hpp"
using namespace std;

void displayMenuBeforeLogin() {
    cout << "\n=== Stock Trading System ===\n";
    cout << "1. Register New User\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
}

void displayMenuAfterLogin() {
    cout << "\n=== Stock Trading System ===\n";
    cout << "1. Add Money to Wallet\n";
    cout << "2. Check Wallet Balance\n";
    cout << "3. View Available Stocks\n";
    cout << "4. Buy Stocks\n";
    cout << "5. Sell Stocks\n";
    cout << "6. View Portfolio & Returns\n";
    cout << "7. Logout\n";
    cout << "8. Delete Account\n";
    cout << "9. Show Transaction History\n";
    cout << "10. Exit\n";
    cout << "Choice: ";
}

string getUserID() {
    string userID;
    cout << "Enter UserID: ";
    getline(cin, userID);
    return userID;
}

string getPassword() {
    string password;
    cout << "Enter Password: ";
    getline(cin, password);
    return password;
}

int main() {
    User user("", "", "");
    Stock stock;
    Transaction transactionManager;
    string choice;

    while(true) {
        if(!user.isLoggedIn()) {
            displayMenuBeforeLogin();
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(choice == "3") {
                cout << "Exiting program.\n";
                break;
            }
            else if(choice == "1") {
                string userID = getUserID();
                string password = getPassword();
                user.registerUser(userID, password);
            }
            else if(choice == "2") {
                string userID = getUserID();
                string password = getPassword();
                if(user.login(userID, password)) {
                    stock.setUserID(userID);
                }
            }
            else {
                cout << "Invalid choice. Select 1-3.\n";
            }
        }
        else {
            displayMenuAfterLogin();
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(choice == "10") {
                cout << "Exiting program.\n";
                break;
            }
            else if(choice == "1") {
                user.addMoneyToWallet(0);
            }
            else if(choice == "2") {
                cout << "Current Wallet Balance: Rs." << user.checkWalletBalance() << endl;
            }
            else if(choice == "3") {
                stock.displayDetails();
            }
            else if(choice == "4") {
                string stockName;
                int quantity;
                cout << "Enter stock symbol (e.g., AAPL): ";
                cin >> stockName;
                cout << "Enter quantity to buy: ";
                cin >> quantity;
                stock.buyStock(stockName, quantity);
            }
            else if(choice == "5") {
                OwnedStock ownedStock;
                ownedStock.setUserID(user.getUserID());
                string stockName;
                int quantity;
                double currentPrice;
                cout << "Enter stock symbol to sell: ";
                cin >> stockName;
                cout << "Enter quantity to sell: ";
                cin >> quantity;
                ownedStock.sell(stockName, quantity);
            }
            else if(choice == "6") {
    OwnedStock ownedStock;
    ownedStock.setUserID(user.getUserID());
    vector<StockMetrics> portfolioMetrics = ownedStock.calculateIndividualReturns();
    
    cout << "\nPortfolio Performance:\n";
    cout << string(80, '-') << endl;
    
    for(const auto& stock : portfolioMetrics) {
        cout << "Stock: " << stock.stockName << endl;
        cout << "Quantity: " << stock.quantity << endl;
        cout << "Average Cost: Rs." << stock.averageCost << endl;
        cout << "Current Price: Rs." << stock.currentPrice << endl;
        cout << "Total Invested: Rs." << stock.totalInvested << endl;
        cout << "Current Value: Rs." << stock.currentValue << endl;
        cout << "Today's Change: Rs." << stock.dayChange << " (" << stock.dayChangePercentage << "%)" << endl;
        //cout << stock.profitLossType << ": Rs." << abs(stock.profitLoss) << endl;
        cout << "Returns: " << stock.profitLossPercentage << "%" << endl;
        //cout << "1 Day Change: " << stock.oneDayChangePercentage << "% (Rs." << stock.oneDayProfitLoss << ")" << endl;
        //cout << "Weekly Change: " << stock.weeklyChangePercentage << "% (Rs." << stock.weeklyProfitLoss << ")" << endl;
        //cout << "52-Week Range: Rs." << stock.lowestPrice52Week << " - Rs." << stock.highestPrice52Week << endl;
        cout << string(80, '-') << endl;
    }
}

            else if(choice == "7") {
                user.logout();
            }
            else if(choice == "8") {
                user.deleteAccount();
            }
            else if(choice == "9") {
            Transaction transactionManager("", "", 0.0, "", "");
            transactionManager.loadTransactions(user.getUserID());
            transactionManager.displayTransaction();
        }


            else {
                cout << "Invalid choice. Select 1-9.\n";
            }
        }

        if(user.isLoggedIn()) {
            cout << "Current User: " << user.getUserID() << endl;
        }
    }
    return 0;
}

