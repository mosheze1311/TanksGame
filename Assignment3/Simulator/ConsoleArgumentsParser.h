#include "ParsedCmdArguments.h"

#include <vector>
#include <string>

class ConsoleArgumentsParser
{
private:
    // === Attributes === //
    std::vector<std::string> arguments;

    // === Private Functions === //
    void initArgumentsVector(int argc, const char * argv[])
    {
        /**
         * @brief Parses command-line arguments into a flat vector of strings.
         *
         * This function processes command-line arguments (e.g., from `main(int argc, char* argv[])`)
         * and stores them in a `std::vector<std::string>`, splitting key-value pairs in the form
         * "key=value" into two separate elements: the key and the value.
         *
         * For example:
         * Input: {"program", "-verbose", "num_threads=4", "=", "algorithm1=main.o"}
         * Output: {"-verbose", "num_threads", "4", "algorithm1", "main.o"}
         *
         * Special handling:
         * - Arguments without "=" are added to the vector as-is.
         * - Arguments containing "=" are split at the first '=' into two separate elements.
         * - Arguments that are exactly "" after the split are ignored.
         *
         * @param argc The number of command-line arguments.
         * @param argv The array of command-line argument strings.
         */
        this->arguments.clear();
        for (int i = 1; i < argc; ++i)
        {
            std::string arg = argv[i];

            size_t idx = arg.find('=');
            if (idx == arg.npos) // no '=' in arg
            {
                arguments.push_back(arg);
                continue;
            }

            std::string arg_name, arg_val;
            arg_name = arg.substr(0, idx);
            arg_val = arg.substr(idx + 1);
            if (arg_name != "")
                arguments.push_back(arg_name);
            if (arg_val != "")
                arguments.push_back(arg_val);
        }
    }
    
    // TODO: make sure the flag is given directly and not just appearing as value for some arg
    SimulatorMode getSimulatorModeFromArgs() const
    {
        for (const std::string &arg : arguments)
        {
            if (arg == "-comparative")
                return SimulatorMode::Comparative;

            if (arg == "-competition")
                return SimulatorMode::Competitive;
        }

        return SimulatorMode::Invalid;
    }

public:
    // === Constructor === //
    ConsoleArgumentsParser(int argc, const char * argv[])
    {
        this->initArgumentsVector(argc, argv);
    }

    // === Parsing Public Method === //
    std::unique_ptr<ParsedCmdArguments> getParsedArguments() const
    {
        SimulatorMode mode = getSimulatorModeFromArgs();

        try
        {
            switch (mode)
            {
            case SimulatorMode::Comparative:
                return std::make_unique<ParsedComparativeCmdArguments>(this->arguments);

            case SimulatorMode::Competitive:
                return std::make_unique<ParsedCompetitiveCmdArguments>(this->arguments);

            case SimulatorMode::Invalid:
            default:
                std::cerr << "Error: Missing or invalid simulator mode flag. Use -comparative or -competition." << std::endl;
                return nullptr;
            }
        }
        catch (const BadArgsException &error)
        {
            std::cerr << error << std::endl;
            return nullptr;
        }
    }
};
