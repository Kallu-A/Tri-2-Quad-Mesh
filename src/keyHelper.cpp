#include <ultimaille/all.h>
#include <set>
#include <string>
#include <map>
#include <string>

// Return all the number from a key except the one in parameter   key "1-5-7-9"
std::vector<int> getAllVerticeFromIntersectKey(std::string intersectKey, int intersectId) {
    std::vector<int> numbers;
    std::string num_str = std::to_string(intersectId);
    
    //transform the string in a list of std::int with each one containiing the value
    size_t pos = 0;
    while ((pos = intersectKey.find("-")) != std::string::npos) {
        int value = std::stoi(intersectKey.substr(0, pos));
        if (value != intersectId) {
            numbers.push_back(value);
        }
        intersectKey.erase(0, pos + 1);
    }
    //add the last value
    int value = std::stoi(intersectKey);
    if (value != intersectId) {
        numbers.push_back(value);
    }

    return numbers;
}


// Helper function to check if a int is in the the string id id looking like "1-2-3"
bool isElementInString(const std::string& str, int element) {
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

//Function that return all key that are in the group
std::vector<std::string> getAllKeyContainNumbers(std::vector<std::string> &keys, std::vector<int> &numbers) {
    std::vector<std::string> keysFilter;

    for (auto key : keys) {
        for (auto number : numbers) {
            if (isElementInString(key, number)) {
                keysFilter.push_back(key);
                break;
            }
        }
    }
    return keysFilter;
}

// Helper function to check if a int is in the the string id id looking like "1-2"
bool isIntinStringBorderOrientationKey(const std::string& str, int borderRegion) {
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