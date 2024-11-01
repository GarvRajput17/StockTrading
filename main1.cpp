#include<bits/stdc++.h>
#include<jni.h>
#include "user.cpp"
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
    cout << "3. Logout\n";
    cout << "4. Delete Account\n";
    cout << "5. Exit\n";
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
    User user("", "", "");  // Initialize with empty strings
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
                user.login(userID, password);
            }
            else {
                cout << "Invalid choice. Select 1-3.\n";
            }
        }
        else {
            displayMenuAfterLogin();
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if(choice == "5") {
                cout << "Exiting program.\n";
                break;
            }
            else if(choice == "1") {
                user.addMoneyToWallet(0);  // Changed to use class method directly
            }
            else if(choice == "2") {
                cout << "Current Wallet Balance: Rs." << user.checkWalletBalance() << endl;
            }
            else if(choice == "3") {
                user.logout();
            }
            else if(choice == "4") {
                user.deleteAccount();
            }
            else {
                cout << "Invalid choice. Select 1-5.\n";
            }
        }

        if(user.isLoggedIn()) {
            cout << "Current User: " << user.getUserID() << endl;
        }
    }
    return 0;
}
