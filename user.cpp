#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <thread>
#include "user.hpp"
#include<utils/utils.hpp>
#include <nlohmann/json.hpp>
//#include"authentication/Auth.hpp"


using namespace std;
using json = nlohmann::json;




void User::saveTransactionToLocal(double amount, const string& paymentMethod) {
    string transactionId = guuid();
    string timestamp = gettime();
    json userData;
    
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        inFile >> userData;
    } else {
        json userData = {
            {"users", json::object()}
        };
    }
    inFile.close();

    json transaction = {
        {"amount", amount},
        {"date", timestamp},
        {"type", paymentMethod},
        {"paymentMethod", paymentMethod}
    };

    userData["users"][userID]["walletBalance"] = walletBalance;
    userData["users"][userID]["transactions"][transactionId] = transaction;

    ofstream outFile("transactions.json");
    outFile << userData.dump(4);
    outFile.close();
}

void User::loadUserData() {
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        json userData;
        inFile >> userData;
        string currentUserID = getUserID();  // Get current logged in user
        
        if (userData["users"].contains(currentUserID)) {
            walletBalance = userData["users"][currentUserID]["walletBalance"].get<double>();
            cout << "Loaded wallet balance: Rs." << walletBalance << endl;
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
    cout << "Amount to be added: Rs." << amount << "\n";
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
    saveTransactionToLocal(amount, paymentMethod);

    cout << "\nPayment successful! ✅\n";
    cout << "Rs." << amount << " added to wallet successfully\n";
    cout << "Current wallet balance: Rs." << walletBalance << "\n";
}

void User::addMoneyToWallet(double amount) {
    cout << "\n=== Add Money to Wallet ===\n";
    int c1 = 100;
    int c2 = 500;
    int c3 = 1000;
    int c4 = 5000;
    int c5 = 10000;
    
    cout << "Choose amount:\n";
    cout << "1. Rs." << c1 << "\n";
    cout << "2. Rs." << c2 << "\n";
    cout << "3. Rs." << c3 << "\n";
    cout << "4. Rs." << c4 << "\n";
    cout << "5. Rs." << c5 << "\n";
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
            cout << "Enter custom amount: Rs.";
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
    ifstream inFile("transactions.json");
    if (inFile.good()) {
        json userData;
        inFile >> userData;
        if (userData["users"].contains(currentUserID)) {
            walletBalance = userData["users"][currentUserID]["walletBalance"].get<double>();
        }
    }
    inFile.close();
    return walletBalance;
}
void User::removeMoneyfromWallet(double amount) {
    loadUserData();  // Load current balance
    if (walletBalance >= amount) {
        walletBalance -= amount;
        
        // Save the updated balance immediately
        json userData;
        ifstream inFile("transactions.json");
        if (inFile.good()) {
            inFile >> userData;
        }
        inFile.close();
        
        userData["users"][userID]["walletBalance"] = walletBalance;
        
        ofstream outFile("transactions.json");
        outFile << setw(4) << userData << endl;
        outFile.close();
        
        cout << "Transaction successful! ✅\n";
        cout << "Your updated wallet balance is Rs." << walletBalance << "\n";
    } else {
        cout << "Insufficient funds. Current balance: Rs." << walletBalance << "\n";
    }
}


string User::getUserID() const {
    return userID;
}

bool User::isLoggedIn() const {
    return isAuthenticated;
}
