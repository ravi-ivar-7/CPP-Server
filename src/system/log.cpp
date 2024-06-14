#include <unordered_map>
#include <string>
#include <iostream>
#include <chrono>
#include <ctime>
#include <boost/filesystem.hpp>

#include "../files/utils.hpp"

namespace fs = boost::filesystem;

void saveLog(std::string logLevel, std::string message)
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_t = std::chrono::system_clock::to_time_t(now);
    std::string timeStr = std::ctime(&now_t);
    timeStr.pop_back(); // remove newline char
    std::string logMessage = "[" + timeStr + "] : " + logLevel + " : " + message;

    std::string filePath = "assets/logs/log.csv";

    writeFile(filePath, logMessage );

    return;
}
