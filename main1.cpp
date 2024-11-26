#include <iostream> // Includes the standard input/output stream library. 
#include <string> // Includes the string library for handling string data. 
#include <limits> // Includes numeric limits for managing stream input. 
#include <chrono> // Includes the chrono library for time utilities. 
#include <thread> // Includes the thread library for managing delays or threading. 
#include <jni.h> // Includes the JNI library for Java Native Interface functions. 
#include "user.hpp" // Includes the header file for the User class. 
#include "stock.hpp" // Includes the header file for the Stock class. 
using namespace std; // Allows usage of standard library functions without prefixing 'std::'.

void displayMenuBeforeLogin() { // Function to display the main menu before login.
    cout << "\n=== Stock Trading System ===\n"; // Prints the header for the menu.
    cout << "1. Register New User\n"; // Option 1: Register a new user.
    cout << "2. Login\n"; // Option 2: Login for existing users.
    cout << "3. Exit\n"; // Option 3: Exit the program.
    cout << "Choice: "; // Prompts the user to input a choice.
}

void displayMenuAfterLogin() { // Function to display the menu after the user logs in.
    cout << "\n=== Stock Trading System ===\n"; // Prints the menu header.
    cout << "1. Add Money to Wallet\n"; // Option 1: Add money to the wallet.
    cout << "2. Check Wallet Balance\n"; // Option 2: Check wallet balance.
    cout << "3. View Available Stocks\n"; // Option 3: View stock details.
    cout << "4. Buy Stocks\n"; // Option 4: Buy stocks.
    cout << "5. Sell Stocks\n"; // Option 5: Sell stocks.
    cout << "6. View Portfolio & Returns\n"; // Option 6: View portfolio and returns.
    cout << "7. Logout\n"; // Option 7: Logout the user.
    cout << "8. Delete Account\n"; // Option 8: Delete user account.
    cout << "9. Show Transaction History\n"; // Option 9: View transaction history.
    cout << "10. Exit\n"; // Option 10: Exit the program.
    cout << "Choice: "; // Prompts the user to input a choice.
}

string getUserID() { // Function to get the user ID from input.
    string userID; // Variable to store user ID.
    cout << "Enter UserID: "; // Prompts user to enter ID.
    getline(cin, userID); // Reads user input, allowing spaces.
    return userID; // Returns the entered user ID.
}

string getPassword() { // Function to get the password from input.
    string password; // Variable to store password.
    cout << "Enter Password: "; // Prompts user to enter password.
    getline(cin, password); // Reads user input for password.
    return password; // Returns the entered password.
}

int main() { // Entry point of the program.
    User user("", "", ""); // Initializes a User object with empty parameters.
    Stock stock; // Initializes a Stock object.
    Transaction transactionManager; // Initializes a Transaction manager object.
    string choice; // Variable to store user's menu choice.

    while(true) { // Infinite loop to handle the main program flow.
        if(!user.isLoggedIn()) { // Checks if user is not logged in.
            displayMenuBeforeLogin(); // Displays the pre-login menu.
            cin >> choice; // Reads the user's choice.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clears the input buffer.

            if(choice == "3") { // If user selects 3, exit program.
                cout << "Exiting program.\n"; // Outputs exit message.
                break; // Breaks the loop and ends the program.
            }
            else if(choice == "1") { // If user selects 1, register a new user.
                string userID = getUserID(); // Gets user ID from input.
                string password = getPassword(); // Gets password from input.
                user.registerUser(userID, password); // Calls registerUser on the User object.
            }
            else if(choice == "2") { // If user selects 2, login process starts.
                string userID = getUserID(); // Gets user ID from input.
                string password = getPassword(); // Gets password from input.
                if(user.login(userID, password)) { // Logs in user and checks success.
                    stock.setUserID(userID); // Sets the user ID for the stock object.
                }
            }
            else { // If the input choice is invalid.
                cout << "Invalid choice. Select 1-3.\n"; // Outputs invalid choice message.
            }
        }
        else { // If user is logged in.
            displayMenuAfterLogin(); // Displays the post-login menu.
            cin >> choice; // Reads the user's choice.
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clears the input buffer.

            if(choice == "10") { // If user selects 10, exit program.
                cout << "Exiting program.\n"; // Outputs exit message.
                break; // Breaks the loop and ends the program.
            }
            else if(choice == "1") { // If user selects 1, add money to wallet.
                user.addMoneyToWallet(0); // Calls addMoneyToWallet method.
            }
            else if(choice == "2") { // If user selects 2, check wallet balance.
                cout << "Current Wallet Balance: Rs." << user.checkWalletBalance() << endl; // Outputs balance.
            }
            else if(choice == "3") { // If user selects 3, display available stocks.
                stock.displayDetails(); // Calls displayDetails on the Stock object.
            }
            else if(choice == "4") { // If user selects 4, buy stocks.
                string stockName; // Variable to store stock symbol.
                int quantity; // Variable to store quantity.
                cout << "Enter stock symbol (e.g., AAPL): "; // Prompts user for stock symbol.
                cin >> stockName; // Reads stock symbol.
                cout << "Enter quantity to buy: "; // Prompts user for quantity.
                cin >> quantity; // Reads quantity.
                stock.buyStock(stockName, quantity); // Calls buyStock with inputs.
            }
            else if(choice == "5") { // If user selects 5, sell stocks.
                OwnedStock ownedStock; // Creates an OwnedStock object.
                ownedStock.setUserID(user.getUserID()); // Sets the user ID for the owned stock.
                string stockName; // Variable to store stock symbol.
                int quantity; // Variable to store quantity.
                cout << "Enter stock symbol to sell: "; // Prompts user for stock symbol.
                cin >> stockName; // Reads stock symbol.
                cout << "Enter quantity to sell: "; // Prompts user for quantity.
                cin >> quantity; // Reads quantity.
                ownedStock.sell(stockName, quantity); // Calls sell on the OwnedStock object.
            }
            else if(choice == "6") { // If user selects 6, view portfolio & returns.
                OwnedStock ownedStock; // Creates an OwnedStock object.
                ownedStock.setUserID(user.getUserID()); // Sets user ID for OwnedStock.
                vector<StockMetrics> portfolioMetrics = ownedStock.calculateIndividualReturns(); // Calculates returns.

                cout << "\nPortfolio Performance:\n"; // Outputs portfolio header.
                cout << string(80, '-') << endl; // Outputs a divider line.

                for(const auto& stock : portfolioMetrics) { // Iterates through portfolio metrics.
                    cout << "Stock: " << stock.stockName << endl; // Outputs stock name.
                    cout << "Quantity: " << stock.quantity << endl; // Outputs quantity.
                    cout << "Average Cost: Rs." << stock.averageCost << endl; // Outputs average cost.
                    cout << "Current Price: Rs." << stock.currentPrice << endl; // Outputs current price.
                    cout << "Total Invested: Rs." << stock.totalInvested << endl; // Outputs total invested.
                    cout << "Current Value: Rs." << stock.currentValue << endl; // Outputs current value.
                    cout << "Today's Change: Rs." << stock.dayChange << " (" << stock.dayChangePercentage << "%)" << endl; // Outputs daily change.
                    //cout << stock.profitLossType << ": Rs." << abs(stock.profitLoss) << endl;
                    cout << "Returns: " << stock.profitLossPercentage << "%" << endl; // Outputs percentage returns.
                    //cout << "1 Day Change: " << stock.oneDayChangePercentage << "% (Rs." << stock.oneDayProfitLoss << ")" << endl;
                    //cout << "Weekly Change: " << stock.weeklyChangePercentage << "% (Rs." << stock.weeklyProfitLoss << ")" << endl;
                    //cout << "52-Week Range: Rs." << stock.lowestPrice52Week << " - Rs." << stock.highestPrice52Week << endl;
                    cout << string(80, '-') << endl; // Outputs a divider line.
                }
            }
            else if(choice == "7") { // If user selects 7, logout.
                user.logout(); // Calls logout method on the User object.
            }
            else if(choice == "8") { // If user selects 8, delete account.
                user.deleteAccount(); // Calls deleteAccount on the User object.
            }
            else if(choice == "9") { // If user selects 9, show transaction history.
                Transaction transactionManager("", "", 0.0, "", ""); // Creates a Transaction object.
                transactionManager.loadTransactions(user.getUserID()); // Loads transactions for the user.
                transactionManager.displayTransaction(); // Displays the transaction history.
            }
            else { // If the input choice is invalid.
                cout << "Invalid choice. Select 1-9.\n"; // Outputs invalid choice message.
            }
        }

        if(user.isLoggedIn()) { // Checks if the user is still logged in.
            cout << "Current User: " << user.getUserID() << endl; // Outputs the current user ID.
        }

        this_thread::sleep_for(chrono::seconds(2)); // Adds a 2-second delay for better UX.
    }

    return 0; // Ends the main function and returns 0.
}
