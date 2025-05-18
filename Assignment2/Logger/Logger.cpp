#include "Logger.h"

using std::string;

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

Logger &Logger::output(const string &file_name)
{
    static Logger instance(file_name);
    return instance;
}

//=== Logging Methods ===
void Logger::logInternal(const string &message, bool newline)
{
    std::lock_guard<std::mutex> lock(logMutex);

    if (logFile.is_open())
    {
        logFile << message;
        if (newline)
            logFile << std::endl;
    }
}

void Logger::log(const string &message)
{
    this->logInternal(message, false);
}
void Logger::logLine(const string &message)
{
    this->logInternal(message, true);
}
