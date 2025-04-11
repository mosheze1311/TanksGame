#pragma once
#include <cstring>
#include <string>
#include <fstream>
#include <mutex>
using namespace std;

class Logger
{
public:
    static Logger &input();
    static Logger &runtime();
    static Logger &output();

    void logError(const std::string &message);
    void logInfo(const std::string &message);

private:
    Logger(const std::string &filename);
    ~Logger();

    std::ofstream logFile;
    std::mutex logMutex;
    void log(const std::string &level, const std::string &message);
};