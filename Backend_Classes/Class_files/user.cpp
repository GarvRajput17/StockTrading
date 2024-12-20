#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "Backend_Classes/Class_files/Header Files/user.hpp"
#include<utils/utils.hpp>
#include <nlohmann/json.hpp>
//#include"authentication/Auth.hpp"
using namespace std;
using json = nlohmann::json;

const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string CYAN = "\033[36m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string MAGENTA = "\033[35m";



void User::saveTransactionToLocal(double amount, const string& paymentMethod) {
    string transactionId = guuid();
    string timestamp = gettime();
    json userData;
    
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        inFile >> userData;
    } else {
        json userData = {
            {"userID", json::object()}
        };
    }
    inFile.close();

    json transaction = {
        {"amount", amount},
        {"date", timestamp},
        {"type", paymentMethod},
        {"paymentMethod", paymentMethod}
    };

    userData[userID]["walletBalance"] = walletBalance;
    userData[userID]["transactions"][transactionId] = transaction;

    ofstream outFile("Databases/test.json");
    outFile << userData.dump(4);
    outFile.close();
}

void User::loadUserData() {
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        json userData;
        inFile >> userData;
        string currentUserID = getUserID();
        if (userData.contains(currentUserID)) {
            walletBalance = userData[currentUserID]["walletBalance"].get<double>();
            cout << "Loaded wallet balance: $" << walletBalance << endl;
        }
    }
    inFile.close();
}

User::User() 
    : userID(""), 
      name(""), 
      password(""), 
      walletBalance(0.00), 
      auth(""),  // Initialize Auth with API key
      isAuthenticated(false) {}

User::User(const string& userID, const string& name, const string& password)
    : userID(userID), 
      name(name), 
      password(password), 
      walletBalance(0.00),
      auth(""),  // Initialize Auth with API key
      isAuthenticated(false) {}

void User::setUserID(const string& id) {
    userID = id;
}


bool User::login(string userID, string password) {
    if (auth.login(userID, password)) {
        this->userID = userID;
        isAuthenticated = true;
        loadUserData();
        cout << "Logged in Successfully" << endl;
        return true;
    }
    cout << "Login failed. Please check your credentials." << endl;
    cout << "If you want to register, please register first." << endl;
    return false;
}

void User::logout() {
    auth.logout();
    userID.clear();
    isAuthenticated = false;
    cout << "Logged out successfully" << endl;
}

bool User::registerUser(string userID, string password) {
    if (auth.registerUser(userID, password)) {
        this->userID = userID;
        isAuthenticated = true;
        
        // Initialize new user data structure in JSON
        json userData;
        ifstream inFile("Databases/test.json");
        if (inFile.good()) {
            inFile >> userData;
        }
        inFile.close();

        // Set up default values for new user
        userData[userID] = {
            {"portfolio", {
                {"OwnedStockId", {
                    {"AveragePrice", 0},
                    {"CurrentPrice", 0},
                    {"Quantity", 0}
                }}
            }},
            {"transactions", json::object()},
            {"walletBalance", 0.0},
            {"watchlist", {
                {"StockId", ""}
            }}
        };

        // Save updated JSON
        ofstream outFile("Databases/test.json");
        outFile << setw(4) << userData << endl;
        outFile.close();

        cout << "Registered successfully!" << endl;
        cout << "Welcome to our platform!" << endl;
        cout << "Start your stock trading journey today!" << endl;
        return true;
    }
    return false;
}


bool User::deleteAccount() {
    if (!isAuthenticated) {
        cout << "Must be logged in to delete account." << endl;
        return false;
    }
    if (auth.deleteAccount()) {
        userID.clear();
        isAuthenticated = false;
        cout << "Account deleted successfully!" << endl;
        return true;
    }
    return false;
}

void User::processPayment(double amount) {
    cout << "\n=== Payment Gateway ===\n";
    cout << "Amount to be added: $" << amount << "\n";
    cout << "Choose payment method:\n";
    cout << "1. Credit Card\n";
    cout << "2. Debit Card\n";
    cout << "3. UPI\n";
    cout << "4. Net Banking\n";
    cout << "5. Digital Wallet\n";
    
    int paymentChoice;
    cout << "Enter payment method (1-5): ";
    cin >> paymentChoice;

    string cardNumber, expiryDate, cvv, name, upiId;
    char saveChoice;
    string paymentMethod;

    switch(paymentChoice) {
        case 1:
        case 2:
            paymentMethod = (paymentChoice == 1) ? "Credit Card" : "Debit Card";
            cout << "\n=== " << paymentMethod << " Payment ===\n";
            cout << "Enter card number (16 digits): ";
            cin >> cardNumber;
            cout << "Enter expiry date (MM/YY): ";
            cin >> expiryDate;
            cout << "Enter CVV: ";
            cin >> cvv;
            cout << "Enter name on card: ";
            cin.ignore();
            getline(cin, name);
            cout << "Save this card for future payments? (Y/N): ";
            cin >> saveChoice;
            break;

        case 3:
            paymentMethod = "UPI";
            cout << "\n=== UPI Payment ===\n";
            cout << "Enter UPI ID: ";
            cin >> upiId;
            cout << "Save this UPI ID for future payments? (Y/N): ";
            cin >> saveChoice;
            break;

        case 4:
            paymentMethod = "Net Banking";
            cout << "\n=== Net Banking ===\n";
            cout << "Select your bank:\n";
            cout << "1. SBI\n2. HDFC\n3. ICICI\n4. Axis\n5. Other\n";
            int bankChoice;
            cin >> bankChoice;
            cout << "Redirecting to bank's secure payment portal...\n";
            break;

        case 5:
            paymentMethod = "Digital Wallet";
            cout << "\n=== Digital Wallet ===\n";
            cout << "Select wallet:\n";
            cout << "1. PayTM\n2. PhonePe\n3. Google Pay\n4. Amazon Pay\n";
            int walletChoice;
            cin >> walletChoice;
            cout << "Enter mobile number linked with wallet: ";
            string mobile;
            cin >> mobile;
            break;
    }

    cout << "\nProcessing payment";
    string dots = "";
    for(int i = 0; i < 3; i++) {
        dots += ".";
        cout << "\rProcessing payment" << dots << string(3 - dots.length(), ' ');
        cout.flush();
        this_thread::sleep_for(chrono::seconds(1));
    }

    walletBalance += amount;
    json userData;
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        inFile >> userData;
    }
    inFile.close();
    
    userData[userID]["walletBalance"] = walletBalance;
    
    // Record transaction
    string transactionId = guuid();
    string timestamp = gettime();
    transactionManager.recordTransaction(userID, transactionId, "CREDIT", amount, timestamp, paymentMethod);
    
    ofstream outFile("Databases/test.json");
    outFile << setw(4) << userData << endl;
    outFile.close();
    

    cout << "\nPayment successful! ✅\n";
    cout << "$" << amount << " added to wallet successfully\n";
    cout << "Current wallet balance: $" << walletBalance << "\n";
}

void User::addMoneyToWallet(double amount) {
    cout << "\n=== Add Money to Wallet ===\n";
    int c1 = 100;
    int c2 = 500;
    int c3 = 1000;
    int c4 = 5000;
    int c5 = 10000;
    
    cout << "Choose amount:\n";
    cout << "1. $" << c1 << "\n";
    cout << "2. $" << c2 << "\n";
    cout << "3. $" << c3 << "\n";
    cout << "4. $" << c4 << "\n";
    cout << "5. $" << c5 << "\n";
    cout << "6. Enter custom amount\n";
    
    int choice;
    cout << "Enter your choice (1-6): ";
    cin >> choice;
    double selectedAmount = 0;

    switch(choice) {
        case 1: selectedAmount = c1; break;
        case 2: selectedAmount = c2; break;
        case 3: selectedAmount = c3; break;
        case 4: selectedAmount = c4; break;
        case 5: selectedAmount = c5; break;
        case 6:
            cout << "Enter custom amount: $";
            cin >> selectedAmount;
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    processPayment(selectedAmount);
}

double User::checkWalletBalance() {
    string currentUserID = getUserID();
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        json userData;
        inFile >> userData;
        if (userData.contains(currentUserID)) {
            walletBalance = userData[currentUserID]["walletBalance"].get<double>();
        }
    }
    inFile.close();
    return walletBalance;
}
void User::removeMoneyfromWallet(double amount) {
    loadUserData(); // Load current balance
    if (walletBalance >= amount) {
        walletBalance -= amount;
        
        // Save the updated balance immediately
        json userData;
        ifstream inFile("Databases/test.json");
        if (inFile.good()) {
            inFile >> userData;
        }
        inFile.close();
        
        userData[userID]["walletBalance"] = walletBalance;
        ofstream outFile("Databases/test.json");
        outFile << setw(4) << userData << endl;
        outFile.close();

        // Record the deduction transaction
        string transactionId = guuid();
        string timestamp = gettime();
        transactionManager.recordTransaction(userID, transactionId, "DEBIT", amount, timestamp, "Wallet Deduction");
        
        cout << "Transaction successful! ✅\n";
        cout << "Your updated wallet balance is $" << walletBalance << "\n";
    } else {
        cout << "Insufficient funds. Current balance: $" << walletBalance << "\n";
    }
}



string User::getUserID() const {
    return userID;
}

bool User::isLoggedIn() const {
    return isAuthenticated;
}

void User::displayTransactionHistory() {
    cout << "\n=== Transaction History ===\n";
    cout << "UserID: " << userID << "\n\n";
    
    Transaction transactionManager("", "", 0.0, "", "");
    //transactionManager.loadTransactions(userID);
    transactionManager.displayTransaction();
}

void User::displayWatchlist() {
    loadUserData();  // Ensure the latest data is loaded

    cout << "\n=== Your Watchlist ===\n";

    // Load `Databases/test.json` to get the user's data and watchlist
    ifstream testFile("Databases/test.json");
    if (!testFile.is_open()) {
        cout << "Failed to load user data. Please try again.\n";
        return;
    }

    json userData;
    testFile >> userData;
    testFile.close();

    // Get the current user ID and check their watchlist
    string currentUserID = getUserID();
    if (!userData.contains(currentUserID) || !userData[currentUserID].contains("watchlist")) {
        cout << "Your watchlist is empty.\n";
        return;
    }

    auto watchlist = userData[currentUserID]["watchlist"];
    if (watchlist["StockId"].empty()) {
        cout << "Your watchlist is empty.\n";
        return;
    }

    string stockId = watchlist["StockId"].get<string>() + ":NASDAQ";



    // Load `Databases/stockdetails.json` to fetch the stock's details
    ifstream stockDetailsFile("Databases/stockdetails.json");
    if (!stockDetailsFile.is_open()) {
        cout << "Failed to load stock details. Please try again.\n";
        return;
    }

    json stockDetails;
    stockDetailsFile >> stockDetails;
    stockDetailsFile.close();

    // Retrieve and display the stock's details
    if (stockDetails.contains(stockId)) {
        cout << "\n" << BOLD << "=== 📊 Stock Details 📈 ===" << RESET << "\n\n";
        
        cout << CYAN << "🔍 Stock ID: " << RESET 
            << stockId << "\n";
        
        cout << GREEN << "💰 Current Price: " << RESET 
            << "$" << stockDetails[stockId]["price_info"]["current_price"] << "\n";
        
        cout << YELLOW << "📊 Price Change: " << RESET 
            << stockDetails[stockId]["price_info"]["price_change"]["amount"]
            << " (" << stockDetails[stockId]["price_info"]["price_change"]["percentage"] << "%)\n";
        
        cout << BLUE << "📈 Movement: " << RESET 
            << stockDetails[stockId]["price_info"]["price_change"]["movement"] << "\n";
        
        cout << MAGENTA << "💱 Currency: " << RESET 
            << stockDetails[stockId]["price_info"]["currency"] << "\n";
            
        cout << "\n" << string(50, '-') << "\n";

    }


     else {
        cout << "Stock details not found for ID: " << stockId << "\n";
    }
}
void User::addToWatchlist(Stock stock) {
    loadUserData();

    json userData;
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        inFile >> userData;
    }
    inFile.close();

    string currentUserID = getUserID();
    if (!userData.contains(currentUserID)) {
        cout << "User data not found.\n";
        return;
    }

    if (!userData[currentUserID].contains("watchlist")) {
        userData[currentUserID]["watchlist"] = json::object();
    }

    string stockName = stock.getstockname();
    userData[currentUserID]["watchlist"]["StockId"] = stockName;

    ofstream outFile("Databases/test.json");
    if (outFile.is_open()) {
        outFile << setw(4) << userData << endl;
        outFile.close();
        cout << "Stock '" << stockName << "' added to your watchlist.\n";
    } else {
        cout << "Failed to update the watchlist. Please try again.\n";
    }
}

void User::removeFromWatchlist(string stockId) {
    loadUserData();  // Ensure the latest data is loaded

    // Open `Databases/test.json` to load user data
    json userData;
    ifstream inFile("Databases/test.json");
    if (inFile.good()) {
        inFile >> userData;
    } else {
        cout << "Failed to load user data. Please try again.\n";
        return;
    }
    inFile.close();

    // Get the current user's data
    string currentUserID = getUserID();
    if (!userData.contains(currentUserID)) {
        cout << "User data not found.\n";
        return;
    }

    // Check if the user has a watchlist and if the stock ID matches
    if (userData[currentUserID].contains("watchlist") &&
        userData[currentUserID]["watchlist"]["StockId"] == stockId) {
        
        // Remove the stock ID from the watchlist
        userData[currentUserID]["watchlist"]["StockId"] = "";

        // Save the updated data back to `Databases/test.json`
        ofstream outFile("Databases/test.json");
        if (outFile.is_open()) {
            outFile << setw(4) << userData << endl;
            outFile.close();
            cout << "Stock with ID '" << stockId << "' removed from your watchlist.\n";
        } else {
            cout << "Failed to update watchlist. Please try again.\n";
        }
    } else {
        cout << "Stock with ID '" << stockId << "' not found in your watchlist.\n";
    }
}



