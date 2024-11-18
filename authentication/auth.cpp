#include "auth.hpp"
#include "Firebase_files/firebase_config.h"
#include <curl/curl.h>

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

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    
    if (method == "POST" || method == "PATCH") {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }

    curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

Auth::Auth(const string& apiKey) : apiKey(apiKey), currentUserId(""), isLoggedIn(false) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
}

bool Auth::registerUser(const string& email, const string& password) {
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + apiKey;
    string data = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    string response = makeRequest(url, data);
    
    if(response.find("idToken") != string::npos) {
        currentUserId = email;
        isLoggedIn = true;
        return true;
    }
    return false;
}

bool Auth::login(const string& email, const string& password) {
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey;
    string data = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    string response = makeRequest(url, data);
    
    if(response.find("idToken") != string::npos) {
        currentUserId = email;
        isLoggedIn = true;
        return true;
    }
    return false;
}

void Auth::logout() {
    isLoggedIn = false;
    currentUserId.clear();
}

bool Auth::deleteAccount() {
    if(!isLoggedIn) return false;
    
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + apiKey;
    string data = "{\"idToken\":\"" + currentUserId + "\"}";
    string response = makeRequest(url, data);
    
    if(response.empty() || response.find("error") == string::npos) {
        currentUserId.clear();
        isLoggedIn = false;
        return true;
    }
    return false;
}
