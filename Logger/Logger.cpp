#include "Logger.h"
#include <iostream>
#include <ctime>

Logger::Logger()
{
    logFile.open("error.log", std::ios::app); // Append mode
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

Logger &Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(const std::string &level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::time_t now = std::time(nullptr);
    char *timeStr = std::asctime(std::localtime(&now));
    timeStr[strlen(timeStr) - 1] = '\0'; // Remove trailing newline

    std::string logEntry = "[" + std::string(timeStr) + "] [" + level + "] " + message;

    std::cerr << logEntry << std::endl; // Console output
    if (logFile.is_open())
    {
        logFile << logEntry << std::endl;
    }
}

void Logger::logError(const std::string &message)
{
    log("ERROR", message);
}

void Logger::logInfo(const std::string &message)
{
    log("INFO", message);
}