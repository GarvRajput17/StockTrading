#include <bits/stdc++.h>
#include "authentication/auth.cpp"
using namespace std;

class User {
private:
    string userID;
    string name;
    string password;
    double walletBalance;
    Auth auth;
    bool isAuthenticated;

public:
    User(const string& userID, const string& name, const string& password)
        : userID(userID), name(name), password(password), walletBalance(0.00), isAuthenticated(false) {}

    // Authentication methods
    bool login(string userID, string password) {
        if (auth.login(userID, password)) {
            this->userID = userID;
            isAuthenticated = true;
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
            cout << "Start you stock trading journey today!" << endl;
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
        cout << "Account deletion failed." << endl;
        return false;
    }

    // Wallet methods
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

        switch(paymentChoice) {
            case 1: // Credit Card
            case 2: // Debit Card
                cout << "\n=== " << (paymentChoice == 1 ? "Credit" : "Debit") << " Card Payment ===\n";
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

            case 3: // UPI
                cout << "\n=== UPI Payment ===\n";
                cout << "Enter UPI ID: ";
                cin >> upiId;
                cout << "Save this UPI ID for future payments? (Y/N): ";
                cin >> saveChoice;
                break;

            case 4: // Net Banking
                cout << "\n=== Net Banking ===\n";
                cout << "Select your bank:\n";
                cout << "1. SBI\n2. HDFC\n3. ICICI\n4. Axis\n5. Other\n";
                int bankChoice;
                cin >> bankChoice;
                cout << "Redirecting to bank's secure payment portal...\n";
                break;

            case 5: // Digital Wallet
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
        for(int i = 0; i < 3; i++) {
            cout << ".";
            this_thread::sleep_for(chrono::seconds(1));
        }
        cout << "\nPayment successful!\n";
        walletBalance += amount;
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

// Helper functions remain the same but include wallet options in displayMenuAfterLogin
