#pragma once

#include <string>
#include <set>
#include <map>
#include <iostream>
#include <sstream>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

enum class SimulatorMode : int
{
    Comparative = 0,
    Competitive = 1,
    Invalid = 2
};

// === Exception Struct for protected:validateArguments === //
class BadArgsException
{
public:
    std::set<std::string> expected_args;
    std::set<std::string> unexpected_args;
    std::set<std::string> missing_values;

    std::map<std::string, std::string> bad_filenames;
    std::map<std::string, std::string> bad_dirs;

    std::string toString() const
    {
        std::ostringstream oss;
        oss << "BadArgsException:\n";

        oss << formatSetSection("  Missing required arguments:\n", expected_args);
        oss << formatSetSection("  Unexpected arguments:\n", unexpected_args);
        oss << formatSetSection("  Arguments with missing values:\n", missing_values);
        oss << formatMapSection("  Arguments with invalid files:\n", bad_filenames);
        oss << formatMapSection("  Arguments with invalid directories:\n", bad_dirs);

        return oss.str();
    }

private:
    static std::string formatSetSection(const std::string &title, const std::set<std::string> &items)
    {
        if (items.empty())
            return "";
        std::ostringstream oss;
        oss << title;
        for (const auto &item : items)
            oss << "    - " << item << "\n";
        return oss.str();
    }

    static std::string formatMapSection(const std::string &title, const std::map<std::string, std::string> &items)
    {
        if (items.empty())
            return "";
        std::ostringstream oss;
        oss << title;
        for (const auto &[key, val] : items)
            oss << "    - " << key << " = " << val << "\n";
        return oss.str();
    }
};


inline std::ostream &operator<<(std::ostream &os, const BadArgsException &ex)
{
    return os << ex.toString();
}

// === Parsed CMD Arguments Classes === //
class ParsedCmdArguments
{

private:
    // === Attributes === //
    SimulatorMode mode = SimulatorMode::Invalid;

    // === Optional Args === //
    int num_threads = 1;
    bool verbose = false;

protected:
    // === CMD Arguments Parsing === //
    void parseFlagFromArgs(const std::string &flag, bool &attr, const std::vector<std::string> &arguments_vec)
    {
        attr = false;
        for (const auto &arg : arguments_vec)
        {
            if (arg == flag)
            {
                attr = true;
                return;
            }
        }
    }
    void parseArgFromArgs(const std::string &name, std::string &attr, const std::vector<std::string> &arguments_vec)
    {
        for (size_t i = 0; i < arguments_vec.size(); ++i)
        {
            if (name == arguments_vec[i] && i + 1 < arguments_vec.size())
            {
                attr = arguments_vec[i + 1];
                return;
            }
        }
        // TODO: maybe raise error if we want it???
    }
    void parseArgFromArgs(const std::string &name, int &attr, const std::vector<std::string> &arguments_vec)
    {
        std::string temp = "";
        this->parseArgFromArgs(name, temp, arguments_vec);
        if (temp == "") // missing arg should not trigger error here - either optional or triggered error in constructor
            return;

        try
        {
            attr = std::stoi(temp);
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Invalid argument for '" << name << "': expected an integer, got '" << temp << "'\n";
            // TODO: maybe raise error if we want it???
        }
        catch (const std::out_of_range &e)
        {
            std::cerr << "Out of range argument for '" << name << "': '" << temp << "' is too large\n";
            // TODO: maybe raise error if we want it???
        }
    }

    // === CMD Arguments Validation === //
    void validateArguments(std::vector<std::string> &arguments_vec)
    {
        std::set<std::string> optional_args = this->getOptionalArgs();
        std::set<std::string> required_args = this->getRequiredArgs();
        std::map<std::string, std::string> dir_args = this->getDirArgs();
        std::map<std::string, std::string> file_args = this->getFileArgs();

        std::set<std::string> expected_args = required_args;
        std::set<std::string> unexpected_args;
        std::set<std::string> missing_values;

        std::map<std::string, std::string> bad_filenames;
        std::map<std::string, std::string> bad_dirs;
        for (size_t i = 0; i < arguments_vec.size(); ++i)
        {
            std::string arg = arguments_vec[i];

            if (!expected_args.contains(arg) && !optional_args.contains(arg))
            {
                unexpected_args.insert(arg);
                continue;
            }

            expected_args.erase(arg);
            if (arg[0] == '-') // arg is a flag - no associated value
            {
                continue;
            }

            if (i + 1 >= arguments_vec.size()) // missing value for last argument
            {
                missing_values.insert(arg);
                continue;
            }

            std::string value = arguments_vec[i + 1];
            if (file_args.contains(arg) &&
                (!this->fileExists(value) || !isMatchingFileExtension(value, file_args[arg]))) // argument value should be a file of correct format
            {
                bad_filenames[arg] = value;
            }

            if (dir_args.contains(arg) && !this->folderHasMatchingFiles(value, dir_args[arg])) // argument value should be a folder with so files
            {
                bad_dirs[arg] = value;
            }
            ++i; // skip the value
        }

        if (!expected_args.empty() || !unexpected_args.empty() || !missing_values.empty() || !bad_filenames.empty() || !bad_dirs.empty())
        {
            throw BadArgsException(
                expected_args,
                unexpected_args,
                missing_values,
                bad_filenames,
                bad_dirs
            );
        }
    }

    virtual std::set<std::string> getOptionalArgs() const
    {
        return {"num_threads", "-verbose"};
    }
    virtual std::set<std::string> getRequiredArgs() const = 0;
    virtual std::map<std::string, std::string> getFileArgs() const = 0;
    virtual std::map<std::string, std::string> getDirArgs() const = 0;

    // === File System Checks for validateArguments === //
    bool fileExists(const std::string &path)
    {
        return fs::exists(path) && fs::is_regular_file(path);
    }

    bool folderHasMatchingFiles(const std::string &path, const std::string &files_extension)
    {
        if (!fs::exists(path) || !fs::is_directory(path))
            return false;

        for (const auto &entry : fs::directory_iterator(path))
        {
            if (fs::is_regular_file(entry) && isMatchingFileExtension(entry.path(), files_extension))
            {
                return true;
            }
        }
        return false;
    }

    bool isMatchingFileExtension(const std::string &path, const std::string &extension)
    {
        fs::path file_path(path);
        std::string file_ext = file_path.extension().string();
        std::transform(file_ext.begin(), file_ext.end(), file_ext.begin(), ::tolower);
        return file_ext == extension;
    }

public:
    // === Constructors === //
    ParsedCmdArguments(SimulatorMode mode, int num_threads, bool verbose)
        : mode(mode), num_threads(num_threads), verbose(verbose) {};

    ParsedCmdArguments(SimulatorMode mode, const std::vector<std::string> &arguments_vec) : mode(mode)
    {
        this->parseArgFromArgs("num_threads", this->num_threads, arguments_vec);
        this->parseFlagFromArgs("-verbose", this->verbose, arguments_vec);
    };

    // === Destructor === //
    virtual ~ParsedCmdArguments() = default;

    // === Getters === //
    SimulatorMode getSimulatorMode() const { return this->mode; }
    int getNumThreads() const { return this->num_threads; }
    bool isVerbose() const { return this->verbose; }

    // === Pure Virtual Usage Message Getter === //
    virtual std::string getUsageDescriptionMessage() const = 0;

    // === toString helper === //
    virtual std::string toString() const
    {
        std::ostringstream oss;
        std::string mode_str = "Invalid";
        if (this->getSimulatorMode() == SimulatorMode::Competitive)
        {
            mode_str = "Competition";
        }
        else if (this->getSimulatorMode() == SimulatorMode::Comparative)
        {
            mode_str = "Comparative";
        }
        oss << "Mode: " << mode_str << "\n"
            << "num_threads: " << num_threads << "\n"
            << "verbose: " << (verbose ? "True" : "False") << "\n";

        return oss.str();
    }
};
class ParsedComparativeCmdArguments : public ParsedCmdArguments
{

private:
    // === Attributes === //
    std::string game_map;
    std::string game_managers_folder;
    std::string algorithm1;
    std::string algorithm2;

protected:
    // === CMD Arguments Validation Setup Overrides === //
    std::set<std::string> getRequiredArgs() const override
    {
        return {"-comparative", "game_map", "game_managers_folder", "algorithm1", "algorithm2"};
    }
    std::map<std::string, std::string> getFileArgs() const override
    {
        return {{"game_map", ".txt"}, {"algorithm1", ".so"}, {"algorithm2", ".so"}};
    }
    std::map<std::string, std::string> getDirArgs() const override
    {
        return {{"game_managers_folder", ".so"}};
    }

public:
    // === Constructors === //
    ParsedComparativeCmdArguments(std::string game_map, std::string game_managers_folder,
                                  std::string algorithm1, std::string algorithm2,
                                  int num_threads, bool verbose)
        : ParsedCmdArguments(SimulatorMode::Comparative, num_threads, verbose),
          game_map(game_map), game_managers_folder(game_managers_folder),
          algorithm1(algorithm1), algorithm2(algorithm2) {}

    ParsedComparativeCmdArguments(std::vector<std::string> arguments_vec) : ParsedCmdArguments(SimulatorMode::Comparative, arguments_vec)
    {
        this->validateArguments(arguments_vec);

        this->parseArgFromArgs("game_map", this->game_map, arguments_vec);
        this->parseArgFromArgs("game_managers_folder", this->game_managers_folder, arguments_vec);
        this->parseArgFromArgs("algorithm1", this->algorithm1, arguments_vec);
        this->parseArgFromArgs("algorithm2", this->algorithm2, arguments_vec);
    };

    // === Destructor === //
    ~ParsedComparativeCmdArguments() override = default;

    // === Getters === //
    std::string getGameMap() const { return this->game_map; }
    std::string getGameManagerFolders() const { return this->game_managers_folder; }
    std::string getAlgorithm1() const { return this->algorithm1; }
    std::string getAlgorithm2() const { return this->algorithm2; }

    // === Usage Message Getter === //
    std::string getUsageDescriptionMessage() const override
    {
        // TODO: write messsage
        return "";
    };

    // === toString helper === //
    std::string toString() const override
    {
        std::ostringstream oss;
        oss << ParsedCmdArguments::toString()
            << "game_map: " << game_map << "\n"
            << "game_managers_folder: " << game_managers_folder << "\n"
            << "algorithm1: " << algorithm1 << "\n"
            << "algorithm2: " << algorithm2 << "\n";
        return oss.str();
    }
};
class ParsedCompetitiveCmdArguments : public ParsedCmdArguments
{
private:
    // === Attributes === //
    std::string game_maps_folder;
    std::string game_manager;
    std::string algorithms_folder;

protected:
    // === CMD Arguments Validation Setup Overrides === //
    std::set<std::string> getRequiredArgs() const override
    {
        return {"-competition", "game_maps_folder", "game_manager", "algorithms_folder"};
    }
    std::map<std::string, std::string> getFileArgs() const override
    {
        return {{"game_manager", ".so"}};
    }
    std::map<std::string, std::string> getDirArgs() const override
    {
        return {{"game_maps_folder", ".txt"}, {"algorithms_folder", ".so"}};
    }

public:
    // === Constructors === //
    ParsedCompetitiveCmdArguments(std::string game_maps_folder, std::string game_manager,
                                  std::string algorithms_folder, int num_threads, bool verbose)
        : ParsedCmdArguments(SimulatorMode::Competitive, num_threads, verbose),
          game_maps_folder(game_maps_folder), game_manager(game_manager),
          algorithms_folder(algorithms_folder) {};

    ParsedCompetitiveCmdArguments(std::vector<std::string> arguments_vec) : ParsedCmdArguments(SimulatorMode::Competitive, arguments_vec)
    {
        this->validateArguments(arguments_vec);

        this->parseArgFromArgs("game_maps_folder", this->game_maps_folder, arguments_vec);
        this->parseArgFromArgs("game_manager", this->game_manager, arguments_vec);
        this->parseArgFromArgs("algorithms_folder", this->algorithms_folder, arguments_vec);
    };

    // === Destructor === //
    ~ParsedCompetitiveCmdArguments() override = default;

    // === Getters === //
    std::string getGameMapsFolder() const { return this->game_maps_folder; }
    std::string getGameManager() const { return this->game_manager; }
    std::string getAlgorithmsFolder() const { return this->algorithms_folder; }

    // === Usage Message Getter === //
    std::string getUsageDescriptionMessage() const override
    {
        // TODO: write messsage
        return "";
    };

    // === toString helper === //
    std::string toString() const override
    {
        std::ostringstream oss;
        oss << ParsedCmdArguments::toString()
            << "game_maps_folder: " << game_maps_folder << "\n"
            << "game_manager: " << game_manager << "\n"
            << "algorithms_folder: " << algorithms_folder << "\n";
        return oss.str();
    }
};
