#include "auth.hpp"
#include "Firebase_files/firebase_config.h"
#include <curl/curl.h>

// WriteCallback function handles the response data from HTTP requests
// contents: Pointer to the received data
// size: Size of each data element
// nmemb: Number of elements
// userp: Pointer to string where response will be stored
// Returns: Total size of processed data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// makeRequest function handles all HTTP communication with Firebase Authentication API
// url: Target endpoint URL for the request
// data: JSON data to be sent in the request body
// method: HTTP method (POST/PATCH/etc.), defaults to POST
// Returns: Server response as string
string makeRequest(const string& url, const string& data, const string& method = "POST") {
    // Initialize CURL library for making HTTP requests
    CURL* curl = curl_easy_init();
    string response;
    if (!curl) return "";

    // Set up HTTP headers for JSON communication
    // Specifies that we're sending and expecting JSON data
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Configure detailed CURL options for the HTTP request
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);          // Disable SSL verification for development
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());            // Set the target URL for the request
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());// Set HTTP method (POST/GET/etc.)
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback); // Function to handle response data
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);        // Where to store the response
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);         // Set request headers

    // For POST or PATCH requests, add the JSON data to the request body
    if (method == "POST" || method == "PATCH") {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
    }

    // Execute the request and clean up resources
    curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    return response;
}

// Auth class constructor initializes authentication state
// apiKey: Firebase project API key for authentication
// Sets up initial state with no logged-in user
Auth::Auth(const string& apiKey) : apiKey(apiKey), currentUserId(""), isLoggedIn(false) {
    curl_global_init(CURL_GLOBAL_DEFAULT);  // Initialize CURL globally for the application
}

// registerUser handles new user registration with Firebase
// email: User's email address
// password: User's chosen password
// Returns: true if registration successful, false otherwise
// Also automatically logs in the user upon successful registration
bool Auth::registerUser(const string& email, const string& password) {
    // Construct the Firebase signup endpoint URL with API key
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:signUp?key=" + apiKey;
    
    // Prepare registration data in JSON format
    string data = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    
    // Make the registration request and check for success token
    string response = makeRequest(url, data);
    if(response.find("idToken") != string::npos) {  // Success check: response contains authentication token
        currentUserId = email;  // Store user email as ID
        isLoggedIn = true;     // Mark user as logged in
        return true;
    }
    return false;
}

// login authenticates existing users with Firebase
// email: User's registered email
// password: User's password
// Returns: true if login successful, false otherwise
bool Auth::login(const string& email, const string& password) {
    // Construct the Firebase signin endpoint URL
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:signInWithPassword?key=" + apiKey;
    
    // Prepare login credentials in JSON format
    string data = "{\"email\":\"" + email + "\",\"password\":\"" + password + "\",\"returnSecureToken\":true}";
    
    // Attempt login and verify success
    string response = makeRequest(url, data);
    if(response.find("idToken") != string::npos) {  // Check for valid authentication token
        currentUserId = email;  // Store authenticated user's email
        isLoggedIn = true;     // Update login state
        return true;
    }
    return false;
}

// logout terminates the current user session
// Clears user ID and resets login state
void Auth::logout() {
    isLoggedIn = false;        // Mark user as logged out
    currentUserId.clear();     // Clear stored user information
}

// deleteAccount removes the current user's account from Firebase
// Returns: true if deletion successful, false otherwise
// Requires user to be logged in
bool Auth::deleteAccount() {
    if(!isLoggedIn) return false;  // Verify user is logged in before attempting deletion
    
    // Construct the account deletion endpoint URL
    string url = "https://identitytoolkit.googleapis.com/v1/accounts:delete?key=" + apiKey;
    
    // Prepare deletion request with user's token
    string data = "{\"idToken\":\"" + currentUserId + "\"}";
    
    // Attempt account deletion and verify success
    string response = makeRequest(url, data);
    if(response.empty() || response.find("error") == string::npos) {
        currentUserId.clear();  // Clear user data
        isLoggedIn = false;    // Update login state
        return true;
    }
    return false;
}