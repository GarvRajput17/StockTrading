#include "HttpClient.hpp"
#include <nlohmann/json.hpp>
using namespace std;

class Auth {
private:
    string apiKey;
    string currentUserId;
    bool isLoggedIn;

public:
    Auth(const string& apiKey);
    bool registerUser(const string& email, const string& password);
    bool login(const string& email, const string& password);
    void logout();
    bool deleteAccount();
};
