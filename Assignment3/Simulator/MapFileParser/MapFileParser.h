#pragma once

#include "../../UserCommon/Logger/Logger.h"
#include "../../UserCommon/Utils/GameObjectTypeUtils.h"
#include "../../UserCommon/BoardSatelliteView/BoardSatelliteView.h"

#include <optional>
#include <functional>
#include <memory>
#include <string>
#include <map>

struct MapReadException
{
};

class MapFileParser
{
private:
    // === Attibutes === //
    std::string file_name;
    std::string map_name;
    size_t width;
    size_t height;
    size_t num_shells;
    size_t max_steps;
    UserCommon_211388913_322330820::BoardSatelliteView sat_view;

    // === Static Attributes === //
    static const char empty_space_char = ' ';

    // === Logger === //
    void logInputError(const std::string &error_message, bool can_recover);

    // === Parse Input Functions === //
    bool parseKeyLine(const std::string &line, size_t line_number, const std::string &expected_key, size_t &out_value);
    bool parseBoardLine(const std::string &line, size_t expected_width, size_t row);

public:
    // === Constructor === //
    MapFileParser(const std::string &input_file_path);

    // === Destructor === //
    ~MapFileParser() = default;

    // === Getters === //
    size_t getWidth() const;
    size_t getHeight() const;
    size_t getNumShells() const;
    size_t getMaxSteps() const;
    std::string getMapName() const;
    std::string getFileName() const;
    const SatelliteView &getSatelliteView() const;
};