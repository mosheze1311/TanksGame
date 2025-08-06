#pragma once

#include "ParsedCmdArguments.h"

#include "MapFileParser/MapFileParser.h"
#include "Registrar/Registrar.h"
#include "ThreadPool/ThreadPool.h"

#include "../common/GameResult.h"

#include "../UserCommon/BoardSatelliteView/BoardSatelliteView.h"
#include "../UserCommon/Logger/Logger.h"

#include <memory>
#include <mutex>
#include <vector>
#include <filesystem>

class Simulator
{
private:
    // === Attributes === //
    std::unique_ptr<ParsedCmdArguments> args;
    std::vector<MapFileParser> maps_details;
    std::map<std::string, GameResult> comparative_results;
    std::map<std::string, size_t> competitive_results;
    std::mutex result_mutex;

    std::vector<void*> so_handlers;

    // === Loading SO Files === //
    void loadAlgorithmFile(const std::string &algorithm_file);
    void loadManagerFile(const std::string &manager_file);
    void loadSOFile(const std::string &file_path);

    // === Read Maps === //
    void readMapFile(const std::string &game_map_file);

    // === Run Modes === //
    void runComparativeMode();
    void runCompetitiveMode();
    void runManagersOnComparativeMode();
    void runManagersOnCompetitiveMode();

    // === Output Files === //
    std::string getComparativeOutputFileName() const;
    std::string createComparativeOutput() const;
    std::vector<std::pair<std::string, std::vector<std::string>>> createSortedGroupedManagers() const;
    
    std::string getCompetitiveOutputFileName() const;
    std::string createCompetitiveOutput() const;

    std::string createManagersGroupLine(const std::string &representative_manager, const std::vector<std::string> &group) const;

    
    // === Comparative MultiThreading === //
    void runSingleComparativeManager(RegistrarEntry<GameManagerFactory> manager_factory_entry);
    void insertComparativeResult(std::string key, GameResult value);

    // === Competitve MultiThreading === //
    void runSingleCompetitiveMatch(size_t map_idx, size_t algorithm_entry_idx, size_t algorithm2_entry_idx);
    void insertCompetitiveResult(std::string key, size_t points);

    // === Shared Functions === //
    std::string getOutputFileName() const;
    void logToOutputFile(const std::string &output_path) const;
    void closeRegistrars();
    void closeSOHandlers();
    std::string getNameForOutputFile(const std::string &path) const;
    

public:
    // === Constructor === //
    explicit Simulator(std::unique_ptr<ParsedCmdArguments> args);

    // Run sim
    void run();
};