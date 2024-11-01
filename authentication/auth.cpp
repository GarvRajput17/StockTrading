#include<bits/stdc++.h>
#include<curl/curl.h>
#include "Firebase_files/firebase_config.h"
#pragma comment(lib, "libcurl")
#pragma comment(lib, "curl")
using namespace std;

//refer to libcurl documentation of c++
class Auth {
private:
    string userid;
    bool isloggedin;

    // helper function to get the data from the server, taken exactly same as given in the documentation
    // contents is the pointer to the data 
    // size is the size of the data
    // nmeb is the number of the data
    // userp  is the pointer to the string obj that keeps the data
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    

    string makeRequest(const string& url, const string& data) {
        CURL* curl = curl_easy_init();
        string response;
        
        if(!curl) {
            return "";
        }

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_perform(curl);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return response;
    }

    bool verifyUserExists(const string& userID) {
        string url = "https://identitytoolkit.googleapis.com/v1/accounts:lookup?key=" + string(FIREBASE_API_KEY);
        string data = "{\"email\":\"" + userID + "\"}";
        string response = makeRequest(url, data);
        return response.find("users") != string::npos;
    }

public:
    Auth() : isloggedin(false) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~Auth() {
        curl_global_cleanup();
    }

    bool login(string userID, string password) {
        /*
        if (!verifyUserExists(userID)) {
            cout << "User does not exist in Firebase" << endl;
            return false;
        }
        */

        string url = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + string(FIREBASE_API_KEY);
        string data = "{\"email\":\"" + userID + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
        string response = makeRequest(url, data);
        
        if(response.find("idToken") != string::npos) {
            userid = userID;
            isloggedin = true;
            cout << "Firebase authentication successful" << endl;
            return true;
        }
        cout << "Firebase authentication failed" << endl;
        return false;
    }

    bool registerUser(string userID, string password) {
        if (verifyUserExists(userID)) {
            cout << "User already exists in Firebase" << endl;
            return false;
        }

        string url = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + string(FIREBASE_API_KEY);
        string data = "{\"email\":\"" + userID + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
        string response = makeRequest(url, data);
        
        if(response.find("idToken") != string::npos) {
            userid = userID;
            isloggedin = true;
            cout << "User successfully registered in Firebase" << endl;
            return true;
        }
        cout << "Firebase registration failed" << endl;
        return false;
    }

    bool deleteAccount() {
        if(!isloggedin || !verifyUserExists(userid)) {
            cout << "User not found in Firebase" << endl;
            return false;
        }
        
        string url = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + string(FIREBASE_API_KEY);
        string data = "{\"idToken\":\"" + userid + "\"}";
        string response = makeRequest(url, data);
        
        if(response.empty()) {
            userid.clear();
            isloggedin = false;
            cout << "User successfully deleted from Firebase" << endl;
            return true;
        }
        cout << "Firebase deletion failed" << endl;
        return false;
    }

    void logout() {
        isloggedin = false;
        userid.clear();
        cout << "Logged out from Firebase" << endl;
    }

    bool isLoggedIn() const {
        return isloggedin;
    }

    string getUserId() const {
        return userid;
    }
};
