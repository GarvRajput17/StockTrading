#include<bits/stdc++.h>
#include<curl/curl.h>
#include "Firebase_files/firebase_config.h"
#pragma comment(lib, "libcurl")
#pragma comment(lib, "curl")
using namespace std;

class Auth {
private:
    string userid;
    bool isloggedin;
    string idToken;

    
    
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
        string data = "{\"idToken\":\"" + userid + "\"}";
        string response = makeRequest(url, data);
        
        // Print response for debugging
        cout << "Verification response: " << response << endl;
        
        return response.find("users") != string::npos;
    }


public:
    Auth() : isloggedin(false) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
    }

    ~Auth() {
        curl_global_cleanup();
    }

    string getFirestoreEndpoint() const {
        return "https://firestore.googleapis.com/v1/projects/" + string(FIREBASE_PROJECT_ID) + "/databases/(default)/documents";
    }

    bool saveToFirestore(const string& collection, const string& document, const string& jsonData) {
    // Replace @ with %40 in email addresses for URL encoding
        string encodedDocument = document;
        size_t atPos = encodedDocument.find("@");
        if(atPos != string::npos) {
            encodedDocument.replace(atPos, 1, "%40");
        }
        
        string documentPath = (encodedDocument[0] == '/') ? encodedDocument : "/" + encodedDocument;
        string url = getFirestoreEndpoint() + "/" + collection + documentPath;
        
        //cout << "DEBUG - Final URL: " << url << endl;
        string response = makeRequest(url, jsonData);
        
       // cout << "\nFirestore Response: " << response << endl;
        if(response.find("error") != string::npos) {
            //cout << "Failed to save data to Firestore" << endl;
            return false;
        }
        
        cout << "Data successfully saved to Firestore" << endl;
        return true;
    }




    bool updateFirestoreDocument(const string& collection, const string& document, const string& jsonData) {
        string url = getFirestoreEndpoint() + "/" + collection + "/" + document;
        string response = makeRequest(url, jsonData);
        
        cout << "\nFirestore Update Response: " << response << endl;
        
        if(response.find("error") != string::npos) {
            cout << "Failed to update document in Firestore" << endl;
            return false;
        }
        
        cout << "Document successfully updated in Firestore" << endl;
        cout << "Collection: " << collection << endl;
        cout << "Document ID: " << document << endl;
        cout << "Updated Data: " << jsonData << endl;
        return true;
    }

    bool deleteFirestoreDocument(const string& collection, const string& document) {
        string url = getFirestoreEndpoint() + "/" + collection + "/" + document;
        string response = makeRequest(url, "");
        
        cout << "\nFirestore Delete Response: " << response << endl;
        
        if(response.find("error") != string::npos) {
            cout << "Failed to delete document from Firestore" << endl;
            return false;
        }
        
        cout << "Document successfully deleted from Firestore" << endl;
        cout << "Collection: " << collection << endl;
        cout << "Document ID: " << document << endl;
        return true;
    }


    bool login(string userID, string password) {
        string url = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + string(FIREBASE_API_KEY);
        string data = "{\"email\":\"" + userID + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
        string response = makeRequest(url, data);
        
        if(response.find("idToken") != string::npos) {
            userid = userID;
            isloggedin = true;
            // Extract and store the ID token
            size_t tokenStart = response.find("\"idToken\":\"") + 11;
            size_t tokenEnd = response.find("\"", tokenStart);
            string idToken = response.substr(tokenStart, tokenEnd - tokenStart);
            
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
        if(!isloggedin) {
            cout << "Must be logged in to delete account." << endl;
            return false;
        }
        
        string password;
        cout << "\n=== Account Deletion ===\n";
        cout << "This action cannot be undone.\n";
        cout << "Enter your password to confirm permanent deletion: ";
        cin >> password;
        
        // Get fresh token for deletion
        string tokenUrl = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + string(FIREBASE_API_KEY);
        string tokenData = "{\"email\":\"" + userid + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
        string tokenResponse = makeRequest(tokenUrl, tokenData);
        
        if(tokenResponse.find("idToken") == string::npos) {
            cout << "Incorrect password. Deletion aborted." << endl;
            return false;
        }
        
        string deleteUrl = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + string(FIREBASE_API_KEY);
        string deleteData = "{\"idToken\":\"" + idToken + "\"}";
        string deleteResponse = makeRequest(deleteUrl, deleteData);
        
        if(deleteResponse.empty() || deleteResponse.find("error") == string::npos) {
            userid.clear();
            isloggedin = false;
            idToken.clear();
            cout << "Account deleted successfully!" << endl;
            return true;
        }
        
        cout << "Account deletion failed." << endl;
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
    void setIdToken(const string& token) {
        idToken = token;
    }
    
    string getIdToken() const {
        return idToken;
    }
};
