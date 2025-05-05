#include "Logger.h"


//===  Constructors ===
Logger::Logger(const std::string &filename)
{
    logFile.open(filename, std::ios::app);
}

Logger::~Logger()
{
    if (logFile.is_open())
    {
        logFile.close();
    }
}

//=== Singleton Accessors ===
Logger &Logger::input()
{
    static Logger instance("input_errors.txt");
    return instance;
}

Logger &Logger::runtime()
{
    static Logger instance("runtime_errors.txt");
    return instance;
}

Logger &Logger::output(const std::string &file_name)
{
    static Logger instance(file_name);
    return instance;
}

//=== Logging Methods ===
void Logger::log(const std::string &level, const std::string &message)
{
    std::lock_guard<std::mutex> lock(logMutex);

    std::string logEntry = "[" + level + "] " + message;
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

// void Logger::logLine(const std::vector<ActionRequest>& tank_actions)
// {
//     std::lock_guard<std::mutex> lock(logMutex);
//     if (logFile.is_open()) {
//         std::string line = vectorToString(tank_actions);
//         logFile << line << std::endl;
//     }
// }

