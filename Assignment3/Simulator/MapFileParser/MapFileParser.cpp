#include "../../UserCommon/Config/ConfigReader.h"
#include "MapFileParser.h"

#include <fstream>
#include <sstream>
#include <iostream>

// === Consturctor === //
MapFileParser::MapFileParser(const std::string &input_file_path)
{
    /*
   return: true iff the board could be initialized using the input file.

   Expected input file format:
   Line 1: Map name or description (can be ignored)
   Line 2: MaxSteps = <NUM>
   Line 3: NumShells = <NUM>
   Line 4: Rows = <NUM>
   Line 5: Cols = <NUM>
   Lines 6 and on: the game map layout

   Notes:
   - Whitespace around '=' is allowed.
   - Map layout lines shorter than Cols will be padded with spaces.
   - Fewer than Rows map lines will be padded with empty lines.
   - Extra lines or columns beyond Rows/Cols will be ignored.
   - Invalid files will be rejected with an error message.

   Allowed map characters:
   - '#' : Wall
   - '@' : Mine
   - '1'–'9' : Tank for Player i (direction will be as decribed in config.h file)
   - Any other character or space is treated as empty space
   */

    std::ifstream file(input_file_path);

    // Handle file open failure
    if (!file)
    {
        MapFileParser::logInputError("Can't open the input file", false);
        throw MapReadException();
    }

    // read map name (name is currently ignored)

    size_t line_number = 1;
    if (!(std::getline(file, map_name)))
    {
        MapFileParser::logInputError("Line 1: file does not contain the map name", false);
        throw MapReadException();
    }

    std::string line;
    // read lines 2-5 with the key-value pairs
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "MaxSteps", max_steps))
        throw MapReadException();
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "NumShells", num_shells))
        throw MapReadException();
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Rows", height))
        throw MapReadException();
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Cols", width))
        throw MapReadException();

    // read the board itself
    this->sat_view.setHeight(this->height);
    this->sat_view.setWidth(this->width);

    size_t board_row_number = 0;
    while (board_row_number < height && std::getline(file, line))
    {

        if (!MapFileParser::parseBoardLine(line, width, board_row_number))
        {
            throw MapReadException(); // return if some error in board - should not happen
        }

        line_number++;      // advance the read line if read any;
        board_row_number++; // advance board row
    }

    // log recoverable errors to file - no exceptions here
    if (board_row_number < height)
    {
        MapFileParser::logInputError("Actual board did not contain enough rows - filled by empty space", true);
    }
    else if (std::getline(file, line))
    {
        MapFileParser::logInputError("Actual board contains too many rows - ignored", true);
    }

    file.close();
};

// === Input File Handling === //
void MapFileParser::logInputError(const std::string &error_message, bool can_recover)
{
    UserCommon_211388913_322330820::Logger::input().logLine(error_message);
    if (!can_recover)
        std::cout << error_message << std::endl;
}

bool MapFileParser::parseKeyLine(const std::string &line, size_t line_number, const std::string &expected_key, size_t &out_value)
{
    size_t equal_pos = line.find("=");
    std::istringstream key_ss(line.substr(0, equal_pos));
    std::istringstream value_ss(line.substr(equal_pos + 1));

    std::string key, equal_sign;
    size_t value;
    if (equal_pos == std::string::npos)
    {
        MapFileParser::logInputError("Line " + std::to_string(line_number) + ": Expected '=' in line", false);
        return false;
    }
    if (!(key_ss >> key))
    {
        MapFileParser::logInputError("Line " + std::to_string(line_number) + ": Missing key.", false);
        return false;
    }
    if (key != expected_key)
    {
        MapFileParser::logInputError("Line " + std::to_string(line_number) + ": Unexpected key '" + key + "', expected '" + expected_key + "'.", false);
        return false;
    }

    if (!(value_ss >> value))
    {
        MapFileParser::logInputError("Line " + std::to_string(line_number) + ": Expected numeric value after '='.", false);
        return false;
    }

    out_value = value;
    return true;
}

bool MapFileParser::parseBoardLine(const std::string &line, size_t expected_width, size_t row)
{
    size_t actual_width = line.length();
    size_t cols = std::min(expected_width, actual_width); // ensure no overflowing lines.
    if (actual_width != expected_width)
        MapFileParser::logInputError("Board line " + std::to_string(row) + ": Expected " + std::to_string(expected_width) + " cols, got " + std::to_string(actual_width), true);

    char obj_char;
    for (size_t col = 0; col < cols; col++)
    {
        obj_char = line[col];
        if (!UserCommon_211388913_322330820::GameObjectTypeUtils::isValidObjectChar(obj_char))
        {
            if (obj_char != MapFileParser::empty_space_char)
                MapFileParser::logInputError("Board line " + std::to_string(row) + ": Invalid char '" + std::string(1, obj_char) + "' in board column " + std::to_string(col) + ". Treated as empty space", true);

            continue;
        }

        this->sat_view.addObjectAt(UserCommon_211388913_322330820::BoardCell(col, row), static_cast<UserCommon_211388913_322330820::GameObjectType>(obj_char));
    }

    return true;
}

// === Getters === //
size_t MapFileParser::getWidth() const
{
    return this->width;
}

size_t MapFileParser::getHeight() const
{
    return this->height;
}

size_t MapFileParser::getNumShells() const
{
    return this->num_shells;
}

size_t MapFileParser::getMaxSteps() const
{
    return this->max_steps;
}
std::string MapFileParser::getMapName() const
{
    return this->map_name;
}
const SatelliteView &MapFileParser::getSatelliteView() const
{
    return this->sat_view;
}
