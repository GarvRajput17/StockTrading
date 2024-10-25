#include<bits/stdc++.h>
#include<openssl/sha.h>
#include<openssl/md5.h>
#include<nlohmann/json.hpp>

using json = nlohmann::json;

class Auth {
private:
    json users;
    const string file = "database.json";
    string hash(const string& password);
    void loadUsers();
    void saveUsers();

public:
    Auth();
    bool signup(const string& username, const string& password);
    bool login(const string& username, const string& password);
    void displayUsers(const string& username);
}
