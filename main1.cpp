#include<bits/stdc++.h>
//#include<jni.h>
#include "user.cpp"
using namespace std;

void displayMenu() {
    cout << "\n Stock Trading Authentication \n";
    cout << "1. Register New User\n";
    cout << "2. Login\n";
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
    User user;
    string choice;

    while(true) {
        displayMenu();
        cin.clear();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(choice == "5") {
            cout << "Exiting program.\n";
            break;
        }

        if(choice == "1") {
            string userID = getUserID();
            string password = getPassword();
            user.registerUser(userID, password);
        }
        else if(choice == "2") {
            string userID = getUserID();
            string password = getPassword();
            user.login(userID, password);
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

        if(user.isLoggedIn()) {
            cout << "Current User: " << user.getUserID() << endl;
        }
    }
    return 0;
}
