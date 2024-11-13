#include "utils.hpp"
#include <chrono>
#include <uuid/uuid.h>
#include <sstream>

string gettime() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return std::ctime(&time);
}

string guuid() {
    uuid_t uuid;
    uuid_generate(uuid);
    char uuid_str[37];
    uuid_unparse_lower(uuid, uuid_str);
    return string(uuid_str);
}
