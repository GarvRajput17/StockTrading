#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#include <curl/curl.h>
#include <string>

class HttpClient {
private:
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

public:
    static std::string post(const std::string& url, const std::string& data);
    static std::string get(const std::string& url);
    static std::string makeRequest(const std::string& url, const std::string& data, const std::string& method);
};

#endif // HTTP_CLIENT_HPP
