#include "Helper.h"
#include "user.hpp"
#include "stock.hpp"
#include "portfolio.hpp"
#include <string>
#include <iostream>
#include <thread> // thread is being used for the loading effect in the terminal 
#include <unistd.h>
#include <chrono>
using namespace std::this_thread;
using namespace std::chrono;

// defining the regexes codes for the colors for the terminal
#define GREEN "\033[32m"
#define RED "\033[31m"
#define BLUE "\033[34m"
#define YELLOW "\033[33m"
#define CYAN "\033[36m"
#define RESET "\033[0m"
#define BOLD "\033[1m"

using namespace std;

// to track the current user and stock 
static User* currentUser = nullptr;
static Stock* currentStock = nullptr;

void displayLogo() {
    cout << CYAN << BOLD << R"(
    ╔═╗╔╦╗╔═╗╔═╗╦╔═  ╔═╗╔═╗┃
    ╚═╗ ║ ║ ║║  ╠╩╗  ║ ╦║ ║┃
    ╚═╝ ╩ ╚═╝╚═╝╩ ╩  ╚═╝╚═╝O!
    )" << RESET << endl;
}


JNIEXPORT void JNICALL Java_Helper_displayMenuBeforeLogin(JNIEnv* env, jobject obj) {
    // JNIEXPORT Is the macro for the Java Native Interface (JNI) that defines the signature of the function.
    // JNIEnv * env is a pointer to the JNI environment.
    // jobject obj is a reference to the Java object that called the native method.
    displayLogo();
    cout << BOLD << "\n=== 📈 StockGo Trading System 📊 ===\n" << RESET;
    cout << GREEN << "1. 🆕 Register New User\n";
    cout << BLUE << "2. 🔑 Login\n";
    cout << RED << "3. 🚪 Exit\n" << RESET;
    cout << YELLOW << "Choice: " << RESET;
}

JNIEXPORT void JNICALL Java_Helper_displayMenuAfterLogin(JNIEnv* env, jobject obj) {
    // JNIEXPORT Is the macro for the Java Native Interface (JNI) that defines the signature of the function.
    // JNIEnv * env is a pointer to the JNI environment.
    // jobject obj is a reference to the Java object that called the native method.
    cout << BOLD << "\n=== 📈 StockGo Trading Menu 📊 ===\n" << RESET;
    cout << "1. 💰 Add Money to Wallet\n";
    cout << "2. 💵 Check Wallet Balance\n";
    cout << "3. 📊 View Available Stocks\n";
    cout << "4. 📁 View Portfolio\n";
    cout << "5. 📈 View Returns\n";
    cout << "6. 📜 Show Transaction History\n";
    cout << "7. ⭐ Add to Watchlist\n";
    cout << "8. 👀 View Watchlist\n";
    cout << "9. 🚪 Logout\n";
    cout << RED << "10. ❌ Delete Account\n" << RESET;
    cout << "11. 🔚 Exit\n";
    cout << YELLOW << "Choice: " << RESET;
}



// Add loading spinner for operations
void showSpinner(const string& message) {
    string spinner = "⠋⠙⠹⠸⠼⠴⠦⠧⠇⠏";
    for(int i = 0; i < 3; i++) {
        for(char c : spinner) {
            cout << "\r" << c << " " << message << "..." << flush;
            usleep(100000);
            // usleep is a unix system call that pauses program execution 
            //for a specified number of microseconds (by default)
            // used here for creating a loading effect in the terminal and showing latency
            //it's used to create deliberate delays for better user experience and system performance.
        }
    }
    cout << "\r✅ " << message << " Complete!" << endl;
}

void loadingEffect(const string& message) {
    cout << BLUE;
    string dots = "...";
    for(int i = 0; i < 3; i++) {
        cout << "\r" << message;
        for(int j = 0; j <= 3; j++) {
            cout << dots.substr(0, j) << flush;
            // dots.substr(0, j) is a string operation that returns a 
            //substring of the string dots,
            // starting at index 0 and ending at index j.
            // This is used to create a loading effect in the terminal.
            // flush is used to print the output immediately to the terminal and avoid buffering.
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
    cout << RESET << endl;
}




JNIEXPORT jstring JNICALL Java_Helper_getUserID(JNIEnv* env, jobject obj) {
    string userID;
    cout << BLUE << BOLD << "🔒 Enter UserID: " << RESET;
    getline(cin, userID);
    return env->NewStringUTF(userID.c_str());
}

JNIEXPORT jstring JNICALL Java_Helper_getPassword(JNIEnv* env, jobject obj) {
    string password;
    cout << BLUE << BOLD << "🔑 Enter Password: " << RESET;
    getline(cin, password);
    return env->NewStringUTF(password.c_str());
    // NewStringUTF is a method that creates a new Java string object from a C-style string.
    // Returns a jstring that Java can work with.
}

JNIEXPORT jboolean JNICALL Java_Helper_registerUser(JNIEnv* env, jobject obj, jstring juserID, jstring jpassword) {
    const char* userID = env->GetStringUTFChars(juserID, 0);
    // getStringUTFChars is a method that returns a pointer to the 
    //underlying C-style string representation of the Java string.
    //Returns a const char* that C++ can work with
    const char* password = env->GetStringUTFChars(jpassword, 0);

    // Java → C++: GetStringUTFChars
    // C++ → Java: NewStringUTF
    
    loadingEffect("Creating your account");
    
    if (!currentUser) currentUser = new User();
    bool result = currentUser->registerUser(userID, password);
    
    if(result) {
        cout << GREEN << "✅ Registration successful! Welcome aboard! 🎉" << RESET << endl;
    } else {
        cout << RED << "❌ Registration failed. Please try again." << RESET << endl;
    }
    
    env->ReleaseStringUTFChars(juserID, userID);
    env->ReleaseStringUTFChars(jpassword, password);
    return result;
}

JNIEXPORT jboolean JNICALL Java_Helper_login(JNIEnv* env, jobject obj, jstring juserID, jstring jpassword) {
    const char* userID = env->GetStringUTFChars(juserID, 0);
    const char* password = env->GetStringUTFChars(jpassword, 0);
    
    loadingEffect("Authenticating");
    
    if (!currentUser) currentUser = new User();
    bool result = currentUser->login(userID, password);
    
    if(result) {
        cout << GREEN << "✅ Login successful! Welcome back! 🎉" << RESET << endl;
        if (!currentStock) {
            currentStock = new Stock();
            currentStock->setUserID(userID);
        }
    } else {
        cout << RED << "❌ Login failed. Please check your credentials." << RESET << endl;
    }
    
    env->ReleaseStringUTFChars(juserID, userID);
    // Releases memory allocated by GetStringUTFChars
    // Must be called after you're done with the C string
    // Prevents memory leaks
    //Takes two parameters:
    //Original Java string (jstring)
    //C string pointer to release
    //Best practice is to always pair:

    //GetStringUTFChars with ReleaseStringUTFChars
    //Like malloc/free or new/delete

    env->ReleaseStringUTFChars(jpassword, password);
    return result;
}

JNIEXPORT void JNICALL Java_Helper_buyStock(JNIEnv* env, jobject obj, jstring jstockName, jint quantity) {
    if (!currentStock) return;
    const char* stockName = env->GetStringUTFChars(jstockName, 0);
    
    cout << CYAN << "🛒 Processing your purchase" << RESET << endl;
    sleep_for(milliseconds(800));
    
    currentStock->buyStock(stockName, quantity);
    env->ReleaseStringUTFChars(jstockName, stockName);
}

JNIEXPORT void JNICALL Java_Helper_sellStock(JNIEnv* env, jobject obj, jstring jstockName, jint quantity) {
    if (!currentStock) return;
    const char* stockName = env->GetStringUTFChars(jstockName, 0);
    
    cout << YELLOW << "💹 Processing your sale" << RESET << endl;
    sleep_for(milliseconds(800));
    
    OwnedStock ownedStock;
    ownedStock.setUserID(currentUser->getUserID());
    ownedStock.sell(stockName, quantity);
    env->ReleaseStringUTFChars(jstockName, stockName);
}

JNIEXPORT void JNICALL Java_Helper_logout(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << BLUE << "🚪 Logging out" << RESET << endl;
        sleep_for(milliseconds(500));
        currentUser->logout();
        cout << GREEN << "✅ Logged out successfully!" << RESET << endl;
    }
}

JNIEXPORT jboolean JNICALL Java_Helper_deleteAccount(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << RED << "⚠️  Processing account deletion" << RESET << endl;
        sleep_for(milliseconds(1000));
        bool result = currentUser->deleteAccount();
        if (result) {
            cout << GREEN << "✅ Account deleted successfully" << RESET << endl;
        }
        return result;
    }
    return false;
}

JNIEXPORT void JNICALL Java_Helper_addMoneyToWallet(JNIEnv* env, jobject obj, jdouble amount) {
    if (currentUser) {
        cout << GREEN << "💰 Adding funds to wallet" << RESET << endl;
        sleep_for(milliseconds(800));
        currentUser->addMoneyToWallet(amount);
        cout << "✅ Funds added successfully!" << endl;
    }
}

JNIEXPORT jdouble JNICALL Java_Helper_checkWalletBalance(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << CYAN << "💵 Fetching wallet balance" << RESET << endl;
        sleep_for(milliseconds(500));
        return currentUser->checkWalletBalance();
    }
    return 0.0;
}

JNIEXPORT void JNICALL Java_Helper_displayStockDetails(JNIEnv* env, jobject obj) {
    if (currentStock) {
        cout << BLUE << "📊 Loading stock details" << RESET << endl;
        sleep_for(milliseconds(800));
        currentStock->displayDetails();
    }
}


JNIEXPORT void JNICALL Java_Helper_displayPortfolio(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << CYAN << "📊 Loading your portfolio" << RESET << endl;
        sleep_for(milliseconds(800));
        Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), currentUser->getUserID());
        portfolio.loadPortfolio();
        cout << BOLD << "\n=== 📈 Your Portfolio Dashboard 📊 ===\n" << RESET;
        portfolio.displayPortfolio();
    }
}

JNIEXPORT void JNICALL Java_Helper_displayReturns(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << BLUE << "📊 Calculating returns" << RESET << endl;
        sleep_for(milliseconds(800));
        
        OwnedStock ownedStock;
        ownedStock.setUserID(currentUser->getUserID());
        vector<StockMetrics> portfolioMetrics = ownedStock.calculateIndividualReturns();
        
        cout << BOLD << "\n=== 📈 Portfolio Performance Report 📊 ===\n" << RESET;
        cout << CYAN << string(80, '-') << RESET << endl;

        
        for(const auto& stock : portfolioMetrics) {
            cout << BOLD << "🏢 Stock: " << RESET << BLUE << stock.stockName << RESET << endl;
            cout << "📦 Quantity: " << stock.quantity << endl;
            cout << "💰 Average Cost: $" << fixed << setprecision(2) << stock.averageCost << endl;
            cout << "💵 Current Price: $" << stock.currentPrice << endl;
            cout << "💸 Total Invested: $" << stock.totalInvested << endl;
            cout << "💎 Current Value: $" << stock.currentValue << endl;
            
            if(stock.profitLossPercentage >= 0) {
                cout << GREEN << "📈 Returns: +" << stock.profitLossPercentage << "%" << RESET << endl;
            } else {
                cout << RED << "📉 Returns: " << stock.profitLossPercentage << "%" << RESET << endl;
            }
            cout << CYAN << string(80, '-') << RESET << endl;

        }
    }
}

JNIEXPORT void JNICALL Java_Helper_showTransactionHistory(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << BLUE << "📜 Loading transaction history" << RESET << endl;
        sleep_for(milliseconds(800));
        Transaction transactionManager("", "", 0.0, "", "");
        transactionManager.loadTransactions(currentUser->getUserID());
        cout << BOLD << "\n=== 📋 Transaction History 📋 ===\n" << RESET;
        transactionManager.displayTransaction();
    }
}

JNIEXPORT void JNICALL Java_Helper_addToWatchlist(JNIEnv* env, jobject obj, jstring jstockName) {
    if (currentUser) {
        const char* stockName = env->GetStringUTFChars(jstockName, 0);
        cout << YELLOW << "⭐ Adding to watchlist" << RESET << endl;
        sleep_for(milliseconds(500));
        
        Stock watchStock;
        watchStock.setStockName(stockName);
        currentUser->addToWatchlist(watchStock);
        
        cout << GREEN << "✅ Stock added to watchlist successfully!" << RESET << endl;
        env->ReleaseStringUTFChars(jstockName, stockName);
    }
}

JNIEXPORT void JNICALL Java_Helper_viewWatchlist(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << BLUE << "👀 Loading watchlist" << RESET << endl;
        sleep_for(milliseconds(800));
        cout << BOLD << "\n=== ⭐ Your Watchlist ⭐ ===\n" << RESET;
        currentUser->displayWatchlist();
    }
}

JNIEXPORT void JNICALL Java_Helper_displayStockOperationsMenu(JNIEnv* env, jobject obj) {
    cout << BOLD << "\n=== 📈 Stock Operations 📊 ===\n" << RESET;
    cout << GREEN << "1. 🛒 Buy Stocks\n";
    cout << RED << "2. 💹 Sell Stocks\n\n";
    cout << BLUE << "3. 📊 View company fundamentals\n";
    cout << YELLOW << "4. 🔙 Back to Main Menu\n" << RESET;
    cout << CYAN << "Choice: " << RESET;
}

JNIEXPORT void JNICALL Java_Helper_handleStockChoice(JNIEnv* env, jobject obj, jstring jchoice) {
    const char* stockChoice = env->GetStringUTFChars(jchoice, 0);
    
    if (strcmp(stockChoice, "3") == 0) {
        cout << BLUE << "Enter the stock symbol to view the Company Fundamentals: " << RESET;
        string stockname;
        cin >> stockname;
        cout << YELLOW << "📊 Loading company fundamentals" << RESET << endl;
        sleep_for(milliseconds(800));
        string command = "java -cp .:lib/json-20240303.jar StockFundamentals.java " + stockname;
        system(command.c_str());
    }
    
    env->ReleaseStringUTFChars(jchoice, stockChoice);
}

JNIEXPORT void JNICALL Java_Helper_displayPortfolioMenu(JNIEnv* env, jobject obj) {
    cout << BOLD << "\n=== 📊 Portfolio Options 📈 ===\n" << RESET;
    cout << "1. 📋 View Portfolio Summary\n";
    cout << GREEN << "2. 📈 Sort by Profit\n";
    cout << RED << "3. 📉 Sort by Loss\n" << RESET;
    cout << "4. 💰 Sort by Investment Amount\n";
    cout << "5. 📊 Sort by Returns\n";
    cout << "6. 🔄 Buy/Sell Stocks\n";
    cout << YELLOW << "7. 🔙 Back to Main Menu\n" << RESET;
    cout << CYAN << "Choice: " << RESET;
}


JNIEXPORT void JNICALL Java_Helper_sortByProfit(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << GREEN << "📈 Sorting portfolio by highest profits" << RESET << endl;
        sleep_for(milliseconds(800));
        Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), currentUser->getUserID());
        portfolio.loadPortfolio();
        portfolio.sortByProfit();
        cout << BOLD << "\n=== 💰 Portfolio Sorted by Profit 📈 ===\n" << RESET;
        portfolio.displayPortfolio();
        portfolio.savePortfolio();
    }
}

JNIEXPORT void JNICALL Java_Helper_sortByLoss(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << RED << "📉 Sorting portfolio by losses" << RESET << endl;
        sleep_for(milliseconds(800));
        Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), currentUser->getUserID());
        portfolio.loadPortfolio();
        portfolio.sortByLoss();
        cout << BOLD << "\n=== 📊 Portfolio Sorted by Loss 📉 ===\n" << RESET;
        portfolio.displayPortfolio();
        portfolio.savePortfolio();
    }
}

JNIEXPORT void JNICALL Java_Helper_sortByInvestment(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << BLUE << "💎 Sorting portfolio by investment amount" << RESET << endl;
        sleep_for(milliseconds(800));
        Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), currentUser->getUserID());
        portfolio.loadPortfolio();
        portfolio.sortByAmountInvested();
        cout << BOLD << "\n=== 💰 Portfolio Sorted by Investment 💰 ===\n" << RESET;
        portfolio.displayPortfolio();
        portfolio.savePortfolio();
    }
}

JNIEXPORT void JNICALL Java_Helper_sortByReturns(JNIEnv* env, jobject obj) {
    if (currentUser) {
        cout << CYAN << "📊 Sorting portfolio by returns" << RESET << endl;
        sleep_for(milliseconds(800));
        Portfolio portfolio(0, 0.0, 0.0, 0.0, vector<OwnedStock>(), currentUser->getUserID());
        portfolio.loadPortfolio();
        portfolio.sortByReturns();
        cout << BOLD << "\n=== ⭐ Portfolio Sorted by Returns 📈 ===\n" << RESET;
        portfolio.displayPortfolio();
        portfolio.savePortfolio();
    }
}



