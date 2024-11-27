// Include necessary headers for time, UUID generation and string handling
#include "utils.hpp"
#include <chrono>      // For system clock and time functions
#include <uuid/uuid.h> // For UUID generation
#include <sstream>     // String stream operations
#include <array>       // Array container
#include <memory>      // Smart pointers
#include <stdexcept>   // Standard exceptions
#include <string>      // String operations
#include <cstdio>      // C-style I/O operations

// Get current system time as formatted string
// Returns: Current time in readable format
string gettime() {
    // Get current time point from system clock
    auto now = std::chrono::system_clock::now();
    // Convert to time_t format
    auto time = std::chrono::system_clock::to_time_t(now);
    // Convert to string format and return
    return std::ctime(&time);
}

// Generate a unique UUID string
// Returns: Lowercase UUID string in standard format (8-4-4-4-12)
string guuid() {
    // Create UUID structure
    uuid_t uuid;
    // Generate random UUID
    uuid_generate(uuid);
    // Buffer for UUID string representation
    char uuid_str[37];  // 36 chars + null terminator
    // Convert UUID to lowercase string format
    uuid_unparse_lower(uuid, uuid_str);
    // Return as C++ string
    return string(uuid_str);
}
