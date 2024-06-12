#include <ultimaille/all.h>
#include <set>
#include <string>
#include <map>
#include <string>

// Helper function to check if a int is in the the string id
static bool isElementInString(const std::string& str, int element) {
    std::string num_str = std::to_string(element);

    // Check if num_str is at the beginning of the string
    if (str.substr(0, num_str.length()) == num_str) {
        if (str.length() == num_str.length() || str[num_str.length()] == '-') {
        return true;
        }
    }

    // Check if num_str is at the end of the string
    if (str.substr(str.length() - num_str.length()) == num_str) {
        if (str.length() == num_str.length() || str[str.length() - num_str.length() - 1] == '-') {
        return true;
        }
    }

    // Check if num_str is in the middle of the string
    size_t pos = str.find(num_str);
    while (pos != std::string::npos) {
        if (pos == 0 || str[pos - 1] == '-') {
        if (str.length() == pos + num_str.length() || str[pos + num_str.length()] == '-') {
            return true;
        }
        }
        pos = str.find(num_str, pos + 1);
    }

    return false;
}

// Helper function to extract two integers from a string
static bool isIntinStringBorderOrientationKey(const std::string& str, int borderRegion) {
    int pos1 = str.find('-');
    if (pos1 == std::string::npos) {
        throw std::invalid_argument("Invalid string format");
    }

    int num1 = std::stoi(str.substr(0, pos1));
    if (num1 == borderRegion )
        return true;
    int num2 = std::stoi(str.substr(pos1 + 1));
    return num2 == borderRegion;
} 