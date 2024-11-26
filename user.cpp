#include <iostream>                             // Include standard input-output stream library
#include <string>                               // Include string library for string manipulation
#include <fstream>                              // Include file stream library for file operations
#include <iomanip>                              // Include iomanip for formatting output
#include <chrono>                               // Include chrono for time-related functions
#include <thread>                               // Include thread for sleep functionality
#include "user.hpp"                             // Include user class header
#include <utils/utils.hpp>                     // Include utility functions
#include <nlohmann/json.hpp>                   // Include nlohmann JSON library for JSON manipulation

using namespace std;                           // Use standard namespace
using json = nlohmann::json;                  // Create an alias for nlohmann::json

// Function to save transaction details to local storage
void User::saveTransactionToLocal(double amount, const string& paymentMethod) {
    string transactionId = guuid();           // Generate a unique transaction ID
    string timestamp = gettime();              // Get the current timestamp
    json userData;                             // JSON object to hold user data
    
    ifstream inFile("test.json");              // Open the JSON file for reading
    if (inFile.good()) {                       // Check if file is good
        inFile >> userData;                    // Read user data from file
    } else {
        json userData = {                      // Initialize user data if file doesn't exist
            {"userID", json::object()}
        };
    }
    inFile.close();                            // Close the input file

    // Create a transaction JSON object
    json transaction = {
        {"amount", amount},
        {"date", timestamp},
        {"type", paymentMethod},
        {"paymentMethod", paymentMethod}
    };

    // Update user data with wallet balance and transaction
    userData[userID]["walletBalance"] = walletBalance;
    userData[userID]["transactions"][transactionId] = transaction;

    ofstream outFile("test.json");            // Open the JSON file for writing
    outFile << userData.dump(4);               // Write updated user data to file with pretty print
    outFile.close();                           // Close the output file
}

// Function to load user data from local storage
void User::loadUser Data() {
    ifstream inFile("test.json");              // Open the JSON file for reading
    if (inFile.good()) {                       // Check if file is good
        json userData;                         // JSON object to hold user data
        inFile >> userData;                    // Read user data from file
        string currentUser ID = getUser ID();    // Get current logged in user
        
        if (userData.contains(currentUser ID)) { // Check if user data exists
            walletBalance = userData[currentUser ID]["walletBalance"].get<double>(); // Load wallet balance
            cout << "Loaded wallet balance: Rs." << walletBalance << endl; // Display loaded balance
        }
    }
    inFile.close();                            // Close the input file
}

// Default constructor for User class
User ::User () 
    : userID(""), 
      name(""), 
      password(""), 
      walletBalance(0.00), 
      auth(""),  // Initialize Auth with API key
      isAuthenticated(false) {}

// Parameterized constructor for User class
User ::User (const string& userID, const string& name, const string& password)
    : userID(userID), 
      name(name), 
      password(password), 
      walletBalance(0.00),
      auth(""),  // Initialize Auth with API key
      isAuthenticated(false) {}

// Setter for userID
void User::setUser ID(const string& id) {
    userID = id;                               // Set userID
}

// Function for user login
bool User::login(string userID, string password) {
    if (auth.login(userID, password)) {       // Authenticate user
        this->userID = userID;                 // Set userID
        isAuthenticated = true;                 // Set authentication status
        loadUser Data();                        // Load user data
        cout << "Logged in Successfully" << endl; // Success message
        return true;
    }
    cout << "Login failed. Please check your credentials." << endl; // Failure message
    cout << "If you want to register, please register first." << endl; // Registration prompt
    return false;
}

// Function for user logout
void User::logout() {
    auth.logout();                             // Logout from authentication
    userID.clear();                            // Clear userID
    isAuthenticated = false;                   // Set authentication status to false
    cout << "Logged out successfully" << endl; // Logout success message
}

// Function for user registration
bool User::registerUser (string userID, string password) {
    if (auth.registerUser (userID, password)) { // Register user
        this->user ID = userID;                               // Set userID
        isAuthenticated = true;                 // Set authentication status
        cout << "Registered successfully!" << endl; // Success message
        cout << "Welcome to our platform!" << endl; // Welcome message
        cout << "Start your stock trading journey today!" << endl; // Encouragement message
        return true;
    }
    return false;                              // Return false if registration fails
}

// Function to delete user account
bool User::deleteAccount() {
    if (!isAuthenticated) {                     // Check if user is logged in
        cout << "Must be logged in to delete account." << endl; // Error message
        return false;
    }
    if (auth.deleteAccount()) {                 // Attempt to delete account
        userID.clear();                         // Clear userID
        isAuthenticated = false;                // Set authentication status to false
        cout << "Account deleted successfully!" << endl; // Success message
        return true;
    }
    return false;                              // Return false if deletion fails
}

// Function to process payment
void User::processPayment(double amount) {
    cout << "\n=== Payment Gateway ===\n";      // Payment gateway header
    cout << "Amount to be added: Rs." << amount << "\n"; // Display amount
    cout << "Choose payment method:\n";        // Prompt for payment method
    cout << "1. Credit Card\n";                 // Option 1
    cout << "2. Debit Card\n";                   // Option 2
    cout << "3. UPI\n";                          // Option 3
    cout << "4. Net Banking\n";                  // Option 4
    cout << "5. Digital Wallet\n";               // Option 5
    
    int paymentChoice;                          // Variable to store payment choice
    cout << "Enter payment method (1-5): ";     // Prompt for choice
    cin >> paymentChoice;                       // Read choice

    string cardNumber, expiryDate, cvv, name, upiId; // Variables for payment details
    char saveChoice;                            // Variable to save choice
    string paymentMethod;                       // Variable to store payment method

    switch(paymentChoice) {                     // Handle payment choice
        case 1:
        case 2:
            paymentMethod = (paymentChoice == 1) ? "Credit Card" : "Debit Card"; // Set payment method
            cout << "\n=== " << paymentMethod << " Payment ===\n"; // Header
            cout << "Enter card number (16 digits): "; // Prompt for card number
            cin >> cardNumber;                   // Read card number
            cout << "Enter expiry date (MM/YY): "; // Prompt for expiry date
            cin >> expiryDate;                  // Read expiry date
            cout << "Enter CVV: ";              // Prompt for CVV
            cin >> cvv;                         // Read CVV
            cout << "Enter name on card: ";     // Prompt for name
            cin.ignore();                       // Ignore newline
            getline(cin, name);                  // Read name
            cout << "Save this card for future payments? (Y/N): "; // Prompt to save card
            cin >> saveChoice;                  // Read save choice
            break;

        case 3:
            paymentMethod = "UPI";               // Set payment method
            cout << "\n=== UPI Payment ===\n";   // Header
            cout << "Enter UPI ID: ";            // Prompt for UPI ID
            cin >> upiId;                        // Read UPI ID
            cout << "Save this UPI ID for future payments? (Y/N): "; // Prompt to save UPI ID
            cin >> saveChoice;                   // Read save choice
            break;

        case 4:
            paymentMethod = "Net Banking";        // Set payment method
            cout << "\n=== Net Banking ===\n";    // Header
            cout << "Select your bank:\n";        // Prompt for bank selection
            cout << "1. SBI\n2. HDFC\n3. ICICI\n4. Axis\n5. Other\n"; // Bank options
            int bankChoice;                       // Variable for bank choice
            cin >> bankChoice;                   // Read bank choice
            cout << "Redirecting to bank's secure payment portal...\n"; // Redirect message
            break;

        case 5:
            paymentMethod = "Digital Wallet";      // Set payment method
            cout << "\n=== Digital Wallet ===\n";  // Header
            cout << "Select wallet:\n";            // Prompt for wallet selection
            cout << "1. PayTM\n2. PhonePe\n3. Google Pay\n4. Amazon Pay\n"; // Wallet options
            int walletChoice;                      // Variable for wallet choice
            cin >> walletChoice;                  // Read wallet choice
            cout << "Enter mobile number linked with wallet: "; // Prompt for mobile number
            string mobile;                        // Variable for mobile number
            cin >> mobile;                        // Read mobile number
            break;
    }

    cout << "\nProcessing payment";               // Processing message
    string dots = "";                            // String for loading dots
    for(int i = 0; i < 3; i++) {                 // Loop for loading animation
        dots += ".";                             // Add dot
        cout << "\rProcessing payment" << dots << string(3 - dots.length(), ' '); // Display loading
        cout.flush();                            // Flush output
        this_thread::sleep_for(chrono::seconds(1)); // Sleep for a second
    }

    walletBalance += amount;                     // Update wallet balance
    json userData;                               // JSON object for user data
    ifstream inFile("test.json");                // Open file for reading
    if (inFile.good()) {                         // Check if file is good
        inFile >> userData;                      // Read user data
    }
    inFile.close();                              // Close input file
    
    userData[userID]["walletBalance"] = walletBalance; // Update wallet balance in user data
    
    // Record transaction
    string transactionId = guuid();              // Generate transaction ID
    string timestamp = gettime();                 // Get current timestamp
    transactionManager.recordTransaction(userID, transactionId, "CREDIT", amount, timestamp, paymentMethod); // Record transaction
    
    ofstream outFile("test.json");               // Open file for writing
    outFile << setw(4) << userData << endl;      // Write updated user data
    outFile.close();                              // Close output file

    cout << "\nPayment successful! ✅\n";        // Success message
    cout << "Rs." << amount << " added to wallet successfully\n"; // Amount added message
    cout << "Current wallet balance: Rs." << walletBalance << "\n"; // Display current balance
}

void User::addMoneyToWallet(double amount) {     // Function to add money to wallet
    cout << "\n=== Add Money to Wallet ===\n";   // Header
    int c1 = 100;                               // Option 1
    int c2 = 500;                               // Option 2
    int c3 = 1000;                              // Option 3
    int c4 = 5000;                              // Option 4
    int c5 = 10000;                             // Option 5
    
    cout << "Choose amount:\n";                 // Prompt for amount
    cout << "1. Rs." << c1 << "\n";             // Option 1
    cout << "2. Rs." << c2 << "\n";             // Option 2
    cout << "3. Rs." << c3 << "\n";             // Option 3
    cout << "4. Rs." << c4 << "\n";             // Option 4
    cout << "5. Rs." << c5 << "\n";             // Option 5
    cout << "6. Enter custom amount\n";         // Option 6
    
    int choice;                                 // Variable for user choice
    cout << "Enter your choice (1-6): ";        // Prompt for choice
    cin >> choice;                              // Read choice
    double selectedAmount = 0;                  // Variable for selected amount

    switch(choice) {                            // Handle user choice
        case 1: selectedAmount = c1; break;     // Set selected amount
        case 2: selectedAmount = c2; break;     // Set selected amount
        case 3: selectedAmount = c3; break;     // Set selected amount
        case 4: selectedAmount = c4; break;     // Set selected amount
        case 5: selectedAmount = c5; break;     // Set selected amount
        case 6:
            cout << "Enter custom amount: Rs."; // Prompt for custom amount
            cin >> selectedAmount;               // Read custom amount
            break;
        default:
            cout << "Invalid choice.\n";        // Invalid choice message
            return;                              // Exit function
    }

    processPayment(selectedAmount);              // Process payment with selected amount
}

double User::checkWalletBalance() {             // Function to check wallet balance
    string currentUser ID = getUser ID();        // Get current user ID
    ifstream inFile("test.json");               // Open file for reading
    if (inFile.good()) {                         // Check if file is good
        json userData;                         // JSON object to hold user data
        inFile >> userData;                    // Read user data from file
        if (userData.contains(currentUser ID)) { // Check if user data exists
            walletBalance = userData[currentUser ID]["walletBalance"].get<double>(); // Load wallet balance
        }
    }
    inFile.close();                            // Close the input file
    return walletBalance;                       // Return the wallet balance
}

void User::removeMoneyfromWallet(double amount) { // Function to remove money from wallet
    loadUser Data(); // Load current balance
    if (walletBalance >= amount) { // Check if sufficient balance
        walletBalance -= amount; // Deduct amount
        
        // Save the updated balance immediately
        json userData; // JSON object for user data
        ifstream inFile("test.json"); // Open file for reading
        if (inFile.good()) { // Check if file is good
            inFile >> userData; // Read user data
        }
        inFile.close(); // Close input file
        
        userData[userID]["walletBalance"] = walletBalance; // Update wallet balance in user data
        ofstream outFile("test.json"); // Open file for writing
        outFile << setw(4) << userData << endl; // Write updated user data
        outFile.close(); // Close output file

        // Record the deduction transaction
        string transactionId = guuid(); // Generate transaction ID
        string timestamp = gettime(); // Get current timestamp
        transactionManager.recordTransaction(userID, transactionId, "DEBIT", amount, timestamp, "Wallet Deduction"); // Record transaction
        
        cout << "Transaction successful! ✅\n"; // Success message
        cout << "Your updated wallet balance is Rs." << walletBalance << "\n"; // Display updated balance
    } else {
        cout << "Insufficient funds. Current balance: Rs." << walletBalance << "\n"; // Insufficient funds message
    }
}

string User::getUser ID() const { // Function to get user ID
    return userID; // Return user ID
}

bool User::isLoggedIn() const { // Function to check if user is logged in
    return isAuthenticated; // Return authentication status
}

void User::displayTransactionHistory() { // Function to display transaction history
    cout << "\n=== Transaction History ===\n"; // Header
    cout << "User ID: " << userID << "\n\n"; // Display user ID
    
    Transaction transactionManager("", "", 0.0, "", ""); // Create transaction manager
    //transactionManager.loadTransactions(userID); // Load transactions (commented out)
    transactionManager.displayTransaction(); // Display transaction history
}
