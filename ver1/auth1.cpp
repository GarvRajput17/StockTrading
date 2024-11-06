#include<bits/stdc++.h>
#include<curl/curl.h>
#include "Firebase_files/firebase_config.h"
#include <nlohmann/json.hpp>
#pragma comment(lib, "libcurl")
#pragma comment(lib, "curl")
using namespace std;
using json = nlohmann::json;

class Auth {
private:
    string userid;
    bool isloggedin;
    string idToken;

    
    
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
        userp->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    string makeRequest(const string& url, const string& data, const string& method = "POST") {
        CURL* curl = curl_easy_init();
        string response;
        if (!curl) return "";

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + idToken).c_str());

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        if (method == "POST" || method == "PATCH") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
        }

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
        }

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

    bool documentExists(const string& collection, const string& document) {
        string url = getFirestoreEndpoint() + "/" + collection + "/" + document;
        string response = makeRequest(url, "", "GET");
        return response.find("error") == string::npos;
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

    /*
    bool saveToFirestore(const string& collection, const string& document, const string& jsonData) {
    // Encode @ with %40 in email addresses
        string encodedDocument = document;
        size_t atPos = encodedDocument.find("@");
        if (atPos != string::npos) {
            encodedDocument.replace(atPos, 1, "%40");
        }
        string url = getFirestoreEndpoint() + "/" + collection + "/" + encodedDocument;
        cout << "DEBUG - Final URL for Firestore request: " << url << endl;

        if (documentExists(collection, encodedDocument)) {
            return updateFirestoreDocument(collection, encodedDocument, jsonData);
        }
        else {
            cout << "Creating document" << endl;
            string response = makeRequest(getFirestoreEndpoint() + "/" + collection, jsonData, "POST");
        }
        cout << "DEBUG - Final URL: " << url << endl;
        string response = makeRequest(url, jsonData);
        
        if (response.find("error") != string::npos) {
            cout << "Failed to save data to Firestore: " << response << endl;
            return false;
        }

        cout << "Data successfully saved to Firestore" << endl;
        return true;
    }




    bool updateFirestoreDocument(const string& collection, const string& document, const string& jsonData) {
        string url = getFirestoreEndpoint() + "/" + collection + "/" + document;

        string response = makeRequest(url, jsonData, "PATCH");
        
        if (response.find("error") != string::npos) {
            cout << "Failed to update document in Firestore: " << response << endl;
            return false;
        }

        cout << "Document successfully updated in Firestore" << endl;
        return true;
    }

    bool deleteFirestoreDocument(const string& collection, const string& document) {
        string url = getFirestoreEndpoint() + "/" + collection + "/" + document;
        string response = makeRequest(url, "", "DELETE");
        
        if (response.find("error") != string::npos) {
            cout << "Failed to delete document from Firestore: " << response << endl;
            return false;
        }

        cout << "Document successfully deleted from Firestore" << endl;
        return true;
    }
    */


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
    /*
    void updateFirestoreAsync() {
    thread([this]() {
        while (true) {
            cout << "DEBUG: Checking for local updates..." << endl;
            
            ifstream file("transactions.json");
            if (file.good()) {
                json localData;
                file >> localData;
                file.close();
                
                if (!localData["transactions"].empty()) {
                    cout << "DEBUG: Found " << localData["transactions"].size() << " transactions to upload" << endl;
                    
                    for (const auto& transaction : localData["transactions"]) {
                        string documentPath = transaction["userId"].get<string>() + "/transactions/" + transaction["transactionId"].get<string>();
                        
                        cout << "DEBUG: Uploading transaction " << transaction["transactionId"].get<string>() << endl;
                        
                        string jsonData = "{\"fields\": {";
                        jsonData += "\"userId\": {\"stringValue\": \"" + transaction["userId"].get<string>() + "\"},";
                        jsonData += "\"amount\": {\"doubleValue\": " + to_string(transaction["amount"].get<double>()) + "},";
                        jsonData += "\"walletBalance\": {\"doubleValue\": " + to_string(transaction["walletBalance"].get<double>()) + "},";
                        jsonData += "\"paymentMethod\": {\"stringValue\": \"" + transaction["paymentMethod"].get<string>() + "\"},";
                        jsonData += "\"timestamp\": {\"stringValue\": \"" + transaction["timestamp"].get<string>() + "\"},";
                        jsonData += "\"transactionId\": {\"stringValue\": \"" + transaction["transactionId"].get<string>() + "\"}";
                        jsonData += "}}";

                        if (saveToFirestore("users", documentPath, jsonData)) {
                            cout << "DEBUG: Successfully uploaded transaction " << transaction["transactionId"].get<string>() << endl;
                        }
                    }

                    ofstream outFile("transactions.json");
                    outFile << "{\"transactions\": []}" << endl;
                    outFile.close();
                    cout << "DEBUG: Cleared local transaction cache" << endl;
                }
            }
            
            cout << "DEBUG: Async update cycle completed. Waiting 30 seconds..." << endl;
            this_thread::sleep_for(chrono::seconds(20));
        }
    }).detach();
}
*/




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
