#include <bits/stdc++.h>
#include "authentication/auth.cpp"
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <utils/uuid.cpp>
#include <utils/chrono.cpp>
#include <Firebase_files/curl_request.cpp>
#include <nlohmann/json.hpp>
#include <fstream>

using namespace std;
using json = nlohmann::json;

class User {
private:
    string userID;
    string name;
    string password;
    double walletBalance;
    Auth auth;
    bool isAuthenticated;

    /*
    void saveTransactionToLocal(double amount, const string& paymentMethod) {
        string transactionId = guuid();
        string timestamp = gettime();
        
        json transaction = {
            {"userId", userID},
            {"amount", amount},
            {"walletBalance", walletBalance},
            {"paymentMethod", paymentMethod},
            {"timestamp", timestamp},
            {"transactionId", transactionId}
        };

        json allTransactions;
        ifstream inFile("transactions.json");
        if (inFile.good()) {
            inFile >> allTransactions;
        }
        inFile.close();

        allTransactions["transactions"].push_back(transaction);

        ofstream outFile("transactions.json");
        outFile << allTransactions.dump(4);
        outFile.close();

        //uploadLocalTransactionsToFirestore();
    }
    */
    /*

    void uploadLocalTransactionsToFirestore() {
        ifstream file("transactions.json");
        if (!file.good()) return;

        json allTransactions;
        file >> allTransactions;
        file.close();

        for (const auto& transaction : allTransactions["transactions"]) {
            string encodedUserID = userID;
            size_t atPos = encodedUserID.find("@");
            if (atPos != string::npos) {
                encodedUserID.replace(atPos, 1, "%40");
            }

            string jsonData = "{\"fields\": {";
            jsonData += "\"userId\": {\"stringValue\": \"" + transaction["userId"].get<string>() + "\"},";
            jsonData += "\"amount\": {\"doubleValue\": " + to_string(transaction["amount"].get<double>()) + "},";
            jsonData += "\"walletBalance\": {\"doubleValue\": " + to_string(transaction["walletBalance"].get<double>()) + "},";
            jsonData += "\"paymentMethod\": {\"stringValue\": \"" + transaction["paymentMethod"].get<string>() + "\"},";
            jsonData += "\"timestamp\": {\"stringValue\": \"" + transaction["timestamp"].get<string>() + "\"},";
            jsonData += "\"transactionId\": {\"stringValue\": \"" + transaction["transactionId"].get<string>() + "\"}";
            jsonData += "}}";

            string documentPath = encodedUserID + "/transactions/" + transaction["transactionId"].get<string>();
            auth.saveToFirestore("users", documentPath, jsonData);
        }

        ofstream outFile("transactions.json");
        outFile << "{\"transactions\": []}";
        outFile.close();
    }
*/

    void saveTransactionToLocal(double amount, const string& paymentMethod) {
        string transactionId = guuid();
        string timestamp = gettime();
        
        // Load existing JSON or create new structure
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

        // Create transaction object
        json transaction = {
        {"amount", amount},
        {"date", timestamp},
        {"type", paymentMethod},  
        {"paymentMethod", paymentMethod}
    };

        // Update user data
        userData["users"][userID]["walletBalance"] = walletBalance;
        userData["users"][userID]["transactions"][transactionId] = transaction;

        // Save updated JSON
        ofstream outFile("transactions.json");
        outFile << userData.dump(4);
        outFile.close();
    }

    // Add this to the login method after authentication success
    void loadUserData() {
        ifstream inFile("transactions.json");
        if (inFile.good()) {
            json userData;
            inFile >> userData;
            
            if (userData["users"].contains(userID)) {
                walletBalance = userData["users"][userID]["walletBalance"];
                cout << "Loaded wallet balance: Rs." << walletBalance << endl;
            }
        }
        inFile.close();
    }

public:
    User(const string& userID, const string& name, const string& password)
        : userID(userID), name(name), password(password), walletBalance(0.00), isAuthenticated(false) {}

    bool login(string userID, string password) {
        if (auth.login(userID, password)) {
            this->userID = userID;
            isAuthenticated = true;
            loadUserData(); // Load user data after successful login
            cout << "Logged in Successfully" << endl;
            return true;
        }
        cout << "Login failed. Please check your credentials." << endl;
        cout << "If you want to register, please register first." << endl;
        return false;
    }

    void logout() {
        auth.logout();
        userID.clear();
        isAuthenticated = false;
        cout << "Logged out successfully" << endl;
    }

    bool registerUser(string userID, string password) {
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

    bool deleteAccount() {
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

    void processPayment(double amount) {
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

        //saveTransactionToLocal(amount, paymentMethod);

        cout << "\nPayment successful! ✅\n";
        cout << "Rs." << amount << " added to wallet successfully\n";
        cout << "Current wallet balance: Rs." << walletBalance << "\n";
    }

    void addMoneyToWallet(double amount) {
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

    double checkWalletBalance() {
        return walletBalance;
    }

    string getUserID() const {
        return userID;
    }

    bool isLoggedIn() const {
        return isAuthenticated;
    }
};

