#pragma once

#include "ParsedCmdArguments.h"
#include "MapFileParser/MapFileParser.h"
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

    // === Loading SO Files === //
    void loadAlgorithmFile(const std::string &algorithm_file);
    void loadManagerFile(const std::string &manager_file);
    void loadSOFile(const std::string& file_path);

    // === Read Maps === //
    void readMapFile(const std::string &game_map_file);

    // === Run Modes === //
    void runComparativeMode();
    void runCompetitiveMode();

    // === Logging Functions === //
    

public:
    // === Constructor === //
    explicit Simulator(std::unique_ptr<ParsedCmdArguments> args);

    // Run sim
    void run();
};