// Logger class for logging messages to different log files (input, runtime, output)
#pragma once
#include <string>
#include <cstring>
#include <fstream>
#include <mutex>

class Logger
{
private:
    //=== Attributes ===
    std::ofstream logFile;
    std::mutex logMutex;
    static Logger output_instance;

    //===  Constructors ===
    Logger(const std::string &filename);
    ~Logger();

    //=== Log Function ===
    void log(const std::string &level, const std::string &message);

public:

    //=== Singleton Accessors ===
    static Logger &input();
    static Logger &runtime();
    static Logger &output(const std::string& file_name);

    //=== Logging Methods ===
    void logError(const std::string &message);
    void logInfo(const std::string &message);
};