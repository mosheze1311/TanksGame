#include "Logger.h"
namespace UserCommon_211388913_322330820
{
    // === Static Members === //
    std::map<std::string, std::unique_ptr<Logger>> Logger::output_map;

    // ===  Constructors === //
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

    // === Singleton Accessors === //
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
        if (!output_map.contains(file_name)){
            output_map.emplace(file_name, std::unique_ptr<Logger>(new Logger(file_name)));
        }

        return *output_map.at(file_name);
    }

    // === Logging Methods === //
    void Logger::logInternal(const std::string &message, bool newline)
    {
        std::lock_guard<std::mutex> lock(logMutex);

        if (logFile.is_open())
        {
            logFile << message;
            if (newline)
                logFile << std::endl;
        }
    }

    void Logger::log(const std::string &message)
    {
        this->logInternal(message, false);
    }
    void Logger::logLine(const std::string &message)
    {
        this->logInternal(message, true);
    }
}