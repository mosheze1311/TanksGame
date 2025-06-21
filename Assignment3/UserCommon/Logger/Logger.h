// Logger class for logging messages to different log files (input, runtime, output)
#pragma once

#include <string>
#include <cstring>
#include <fstream>
#include <mutex>

class Logger
{
private:
    // === Attributes ===  //
    std::ofstream logFile;
    std::mutex logMutex;

    // ===  Constructors === //
    Logger(const std::string &filename);

    // === Destructor === //
    ~Logger();
    
    // === Copy & Move Constructors, Operators (Deleted) === //
    Logger(const Logger &other) = delete;
    Logger operator=(const Logger &other) = delete;
    Logger(const Logger &&other) = delete;
    Logger operator=(const Logger &&other) = delete;

    // === Log Function === //
    void logInternal(const std::string &message, bool newline);

public:
    // === Singleton Accessors === //
    static Logger &input();
    static Logger &runtime();
    static Logger &output(const std::string &file_name);

    // === Logging Methods === //
    void log(const std::string &message);
    void logLine(const std::string &message);
};