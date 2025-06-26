#pragma once

#include "ParsedCmdArguments.h"
#include "MapFileParser/MapFileParser.h"

#include "../common/GameResult.h"

#include "../UserCommon/BoardSatelliteView/BoardSatelliteView.h"
#include "../UserCommon/Logger/Logger.h"

#include <memory>
#include <vector>
#include <iostream>
#include <filesystem>

class Simulator
{
private:
    // === Attributes === //
    std::unique_ptr<ParsedCmdArguments> args;
    std::vector<MapFileParser> maps_details;
    std::map<std::string, GameResult> comparative_results;
    std::map<std::string, size_t> competitive_results;

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
    void createComparativeOutput() const;
    std::vector<std::pair<std::string, std::vector<std::string>>> createSortedGroupedManagers(const std::map<std::string, GameResult> &managers_results) const;
    
    std::string getCompetitiveOutputFileName() const;
    void createCompetitiveOutput() const;

    void logManagerGroup(const std::string &representative_manager, const std::vector<std::string> &group,
                        const std::string &comparative_output_file) const;

public:
    // === Constructor === //
    explicit Simulator(std::unique_ptr<ParsedCmdArguments> args);

    // Run sim
    void run();
};