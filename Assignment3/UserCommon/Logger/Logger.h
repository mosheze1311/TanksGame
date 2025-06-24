// Logger class for logging messages to different log files (input, runtime, output)
#pragma once


#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace UserCommon_211388913_322330820
{
    class Logger
    {
    private:
        // === Attributes ===  //
        std::ofstream logFile;
        std::mutex logMutex;

         // === Static Members === //
        static std::map<std::string, std::unique_ptr<Logger>> output_map;
        friend class std::default_delete<Logger>;

        // ===  Constructors === //
        Logger(const std::string &filename);

        // === Destructor === //
        ~Logger();

        // === Copy & Move Constructors, Operators (Deleted) === //
        Logger(const Logger &other) = delete;
        Logger& operator=(const Logger &other) = delete;
        Logger(Logger &&other) = delete;
        Logger &operator=(Logger &&other) = delete;

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
}