#include <iostream>
#include <vector>
#include <filesystem>

#if defined(__unix__) || defined(__APPLE__)

#include <unistd.h>

std::string getExecutablePath() {
    char pBuf[256];
    size_t len = sizeof(pBuf);

    // int bytes = MIN(readlink("/proc/self/exe", pBuf, len), len - 1);
    int bytes = readlink("/proc/self/exe", pBuf, len);

    if(bytes >= 0)
        pBuf[bytes] = '\0';
    
    std::string path(pBuf);

    // Split path, get directory name without program name
    int i = path.find_last_of("/");
    return path.substr(0, i);
}

#elif defined(_WIN32)

#define NOMINMAX   
#include <Windows.h>
#include <iomanip>
#include "../../../../../usr/include/c++/11/bits/fs_ops.h"
std::string getExecutablePath() {
    char pBuf[256];
    size_t len = sizeof(pBuf);
    int bytes = GetModuleFileName(NULL, pBuf, len);

    std::string path(pBuf);

    // Split path, get directory name without program name
    int i = path.find_last_of("\\");
    return path.substr(0, i);
}

#endif

std::string getAssetPath() {
    std::string path = getExecutablePath();
    #if defined(__unix__) || defined(__APPLE__)
    path += "/mesh/";
    #elif defined(_WIN32)
    // Considering user will use VS C++ on Windows,
    // it will generate a target directory according to the config (Release, Debug, ...)
    // We get assets on the top directory
    path += "\\..\\mesh\\";
    #endif
    return path;
}

std::string getGraphitePath() {
    std::string path = getExecutablePath();
    #if defined(__unix__) || defined(__APPLE__)
    path += "/../GraphiteThree/bin/win64/graphite.exe";
    #elif defined(_WIN32)
    // Considering user will use VS C++ on Windows,
    // it will generate a target directory according to the config (Release, Debug, ...)
    // We get assets on the top directory
    path += "\\..\\..\\GraphiteThree\\bin\\win64\\graphite.exe";
    #endif
    return path;
}


void createDirectory(const std::string& dirName) {
    std::error_code ec;
    std::filesystem::create_directories(dirName, ec);

    if (ec) {
        std::cerr << "Error creating directory: " << ec.message() << std::endl;
    }
}

void emptyDirectory(const std::string& dirName) {
    std::error_code ec;
    std::filesystem::remove_all(dirName, ec);

    if (ec) {
        std::cerr << "Error emptying directory: " << ec.message() << std::endl;
    }
}

void deleteFilesWithExtension(const std::string& dirName, const std::string& ext) {
    std::error_code ec;
    for (const auto& entry : std::filesystem::directory_iterator(dirName, ec)) {
        if (ec) {
            std::cerr << "Error iterating through directory: " << ec.message() << std::endl;
            return;
        }
        if (entry.is_regular_file() && entry.path().extension() == ext) {
            std::filesystem::remove(entry.path(), ec);
            if (ec) {
                std::cerr << "Error deleting file: " << ec.message() << std::endl;
            }
        }
    }
}

bool is_equal(double a, double b, double epsilon = 1e-9) {
    return std::abs(a - b) <= epsilon;
}

std::string convertToNumberId(int number) {
    std::string name = std::to_string(number);

    // pad the string with leading zeros
    std::stringstream ss;
    ss << std::setw(5) << std::setfill('0') << name;
    return ss.str();

}

bool isElementInVector(const std::vector<int>& vec, int element) {
    return (std::find(vec.begin(), vec.end(), element) != vec.end());
}