#include <curl/curl.h>
#include <string>

class HttpClient {
public:
    static std::string post(const std::string& url, const std::string& data);
    static std::string get(const std::string& url);
};
