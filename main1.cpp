#include <iostream>
#include <string>
#include <limits>
#include <chrono>
#include <thread>
#include <jni.h>
#include "Backend_Classes/Class_files/Header Files/user.hpp"
#include "Backend_Classes/Class_files/Header Files/stock.hpp"
#include "Backend_Classes/Class_files/Header Files/portfolio.hpp"
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
    cout << "4. View Portfolio\n";
    cout << "5. View Returns\n";
    cout << "6. Show Transaction History\n";
    cout << "7. Add to Watchlist\n";
    cout << "8. View Watchlist\n";
    cout << "9. Logout\n";
    cout << "10. Delete Account\n";
    cout << "11. Exit\n";
    cout << "Choice: ";
}

void displayStockOperationsMenu() {
    cout << "\n=== Stock Operations ===\n";
    cout << "1. Buy Stocks\n";
    cout << "2. Sell Stocks\n";
    cout << "\n";
    cout << "3. View company fundamentals\n";
    cout << "4. Back to Main Menu\n";
    cout << "Choice: ";
}

void displayPortfolioMenu() {
    cout << "\n=== Portfolio Options ===\n";
    cout << "1. View Portfolio Summary\n";
    cout << "2. Sort by Profit\n";
    cout << "3. Sort by Loss\n";
    cout << "4. Sort by Investment Amount\n";
    cout << "5. Sort by Returns\n";
    cout << "6. Buy/Sell Stocks\n";
    cout << "7. Back to Main Menu\n";
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
    Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), "");

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

            if(choice == "11") {
                cout << "Exiting program.\n";
                break;
            }
            else if(choice == "1") {
                user.addMoneyToWallet(0);
            }
            else if(choice == "2") {
    cout << "Current Wallet Balance: $" << user.checkWalletBalance() << endl;
}
            else if(choice == "3") {
                stock.displayDetails();
                string stockChoice;
                displayStockOperationsMenu();
                cin >> stockChoice;
                
                if(stockChoice == "1") {
                    string stockName;
                    int quantity;
                    cout << "Enter stock symbol (e.g., AAPL): ";
                    cin >> stockName;
                    cout << "Enter quantity to buy: ";
                    cin >> quantity;
                    stock.buyStock(stockName, quantity);
                }
                else if(stockChoice == "2") {
                    OwnedStock ownedStock;
                    ownedStock.setUserID(user.getUserID());
                    string stockName;
                    int quantity;
                    cout << "Enter stock symbol to sell: ";
                    cin >> stockName;
                    cout << "Enter quantity to sell: ";
                    cin >> quantity;
                    ownedStock.sell(stockName, quantity);
                }
                else if (stockChoice == "3") {
                    cout << "Enter the stock symbol to view the Company Fundamentals: ";
                    string stockname;
                    cin >> stockname;
                    string command = "java -cp .:lib/json-20240303.jar StockFundamentals.java " + stockname ;
                    system(command.c_str());
                }
                else if (stockChoice == "4") {
                    displayMenuAfterLogin();
                }
                else {
                    cout << "Invalid choice. Select 1-4.\n";
                }
            }
            else if(choice == "4") {
                Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), user.getUserID());
                portfolio.loadPortfolio();
                
                string portfolioChoice;
                do {
                    displayPortfolioMenu();
                    cin >> portfolioChoice;
                    
                    if(portfolioChoice == "1") {
                        portfolio.displayPortfolio();
                    }
                    else if(portfolioChoice == "2") {
                        portfolio.sortByProfit();
                        portfolio.displayPortfolio();
                    }
                    else if(portfolioChoice == "3") {
                        portfolio.sortByLoss();
                        portfolio.displayPortfolio();
                    }
                    else if(portfolioChoice == "4") {
                        portfolio.sortByAmountInvested();
                        portfolio.displayPortfolio();
                    }
                    else if(portfolioChoice == "5") {
                        portfolio.sortByReturns();
                        portfolio.displayPortfolio();
                    }
                    else if(portfolioChoice == "6") {
                        displayStockOperationsMenu();
                        string opChoice;
                        cin >> opChoice;
                        
                        if(opChoice == "1") {
                            string stockName;
                            int quantity;
                            cout << "Enter stock symbol (e.g., AAPL): ";
                            cin >> stockName;
                            cout << "Enter quantity to buy: ";
                            cin >> quantity;
                            stock.buyStock(stockName, quantity);
                        }
                        else if(opChoice == "2") {
                            OwnedStock ownedStock;
                            ownedStock.setUserID(user.getUserID());
                            string stockName;
                            int quantity;
                            cout << "Enter stock symbol to sell: ";
                            cin >> stockName;
                            cout << "Enter quantity to sell: ";
                            cin >> quantity;
                            ownedStock.sell(stockName, quantity);
                        }
                    }
                } while(portfolioChoice != "7");
            }
            else if(choice == "5") {
    OwnedStock ownedStock;
    ownedStock.setUserID(user.getUserID());
    vector<StockMetrics> portfolioMetrics = ownedStock.calculateIndividualReturns();
    cout << "\nPortfolio Performance:\n";
    cout << string(80, '-') << endl;
    for(const auto& stock : portfolioMetrics) {
        cout << "Stock: " << stock.stockName << endl;
        cout << "Quantity: " << stock.quantity << endl;
        cout << "Average Cost: $" << stock.averageCost << endl;
        cout << "Current Price: $" << stock.currentPrice << endl;
        cout << "Total Invested: $" << stock.totalInvested << endl;
        cout << "Current Value: $" << stock.currentValue << endl;
        cout << "Today's Change: $" << stock.dayChange << " (" << stock.dayChangePercentage << "%)" << endl;
        cout << "Returns: " << stock.profitLossPercentage << "%" << endl;
        cout << string(80, '-') << endl;
    }
}

            else if(choice == "6") {
                Transaction transactionManager("", "", 0.0, "", "");
                transactionManager.loadTransactions(user.getUserID());
                transactionManager.displayTransaction();
            }
            else if(choice == "7") {
                string stockName;
                cout << "Enter stock symbol to add to watchlist: ";
                cin >> stockName;
                Stock watchStock;
                watchStock.setStockName(stockName);
                user.addToWatchlist(watchStock);
            }
            else if(choice == "8") {
                user.displayWatchlist();
            }
            else if(choice == "9") {
                user.logout();
            }
            else if(choice == "10") {
                user.deleteAccount();
            }
            else {
                cout << "Invalid choice. Please select a valid option.\n";
            }
        }

        if(user.isLoggedIn()) {
            cout << "Current User: " << user.getUserID() << endl;
        }
    }
    return 0;
}


