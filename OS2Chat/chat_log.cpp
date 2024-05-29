#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <vector>

std::mutex logMutex;
std::string chatLogFile = "chat_log.txt";

void logMessage(const std::string& clientInfo, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ofstream logFile(chatLogFile, std::ios::app);
    if (logFile.is_open()) {
        logFile << clientInfo << ": " << message << std::endl;
        logFile.close();
    } else {
        std::cerr << "Unable to open log file" << std::endl;
    }
}

std::string getChatLog() {
    std::lock_guard<std::mutex> lock(logMutex);
    std::ifstream logFile(chatLogFile);
    std::string log((std::istreambuf_iterator<char>(logFile)), std::istreambuf_iterator<char>());
    return log;
}
