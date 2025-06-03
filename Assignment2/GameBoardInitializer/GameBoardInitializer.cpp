#include "../Config/ConfigReader.h"
#include "GameBoardInitializer.h"

#include <fstream>
#include <sstream>

// === Input File Handling === //
void GameBoardInitializer::logInputError(const std::string &error_message)
{
    Logger::input().logLine(error_message);
}

bool GameBoardInitializer::parseKeyLine(const std::string &line, size_t line_number, const std::string &expected_key, size_t &out_value)
{
    std::istringstream iss(line);
    std::string key, equal_sign;
    size_t value;

    if (!(iss >> key))
    {
        GameBoardInitializer::logInputError("Line " + std::to_string(line_number) + ": Missing key.");
        return false;
    }
    if (key != expected_key)
    {
        GameBoardInitializer::logInputError("Line " + std::to_string(line_number) + ": Unexpected key '" + key + "', expected '" + expected_key + "'.");
        return false;
    }
    if (!(iss >> equal_sign) || equal_sign != "=")
    {
        GameBoardInitializer::logInputError("Line " + std::to_string(line_number) + ": Expected '=' after key.");
        return false;
    }

    if (!(iss >> value))
    {
        GameBoardInitializer::logInputError("Line " + std::to_string(line_number) + ": Expected numeric value after '='.");
        return false;
    }

    out_value = value;
    return true;
}

bool GameBoardInitializer::parseBoardLine(const std::string &line, size_t expected_width, size_t row, GameBoard &board, std::function<void(std::unique_ptr<GameObject>, const BoardCell &)> addObjectToBoard)
{
    size_t actual_width = line.length();
    size_t cols = std::min(expected_width, actual_width); // ensure no overflowing lines.
    if (actual_width != expected_width)
        GameBoardInitializer::logInputError("Board line " + std::to_string(row) + ": Expected " + std::to_string(expected_width) + " cols, got " + std::to_string(actual_width));

    char obj_char;
    for (size_t col = 0; col < cols; col++)
    {
        obj_char = line[col];
        if (!GameObjectTypeUtils::isValidObjectChar(obj_char))
        {
            if (obj_char != GameBoardInitializer::empty_space_char)
                GameBoardInitializer::logInputError("Board line " + std::to_string(row) + ": Invalid char '" + std::string(1, obj_char) + "' in board column " + std::to_string(col) + ". Treated as empty space");

            continue;
        }

        std::unique_ptr<GameObject> ptr = createGameObjectOfType(board, static_cast<GameObjectType>(obj_char));
        addObjectToBoard(std::move(ptr), BoardCell(col, row));
    }

    return true;
}

std::unique_ptr<GameObject> GameBoardInitializer::createGameObjectOfType(GameBoard &board, GameObjectType type)
{
    switch (type)
    {
    case GameObjectType::TANK1:
    case GameObjectType::TANK2:
    case GameObjectType::TANK3:
    case GameObjectType::TANK4:
    case GameObjectType::TANK5:
    case GameObjectType::TANK6:
    case GameObjectType::TANK7:
    case GameObjectType::TANK8:
    case GameObjectType::TANK9:
        return std::make_unique<Tank>(
            board,
            type,
            ConfigReader::getConfig().getSpawnDirections()[GameObjectTypeUtils::tankTypeToPlayerIndex(type) - 1],
            board.getTanksNumShells());

    case GameObjectType::WALL:
        return std::make_unique<Wall>(board);

    case GameObjectType::MINE:
        return std::make_unique<Mine>(board);

    case GameObjectType::SHELL:
        return std::make_unique<Shell>(board, Direction::DOWN);
        // Should never happen, input file should not contain shells.

    default:
        return nullptr; // should not get here
    }
}

bool GameBoardInitializer::initGameBoardFromFile(const std::string &input_file_path,
                                                 GameBoard &board,
                                                 std::function<void(int, int, int, int)> setBoardDetails,
                                                 std::function<void(std::unique_ptr<GameObject>, const BoardCell &)> addObjectToBoard)
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
        GameBoardInitializer::logInputError("Can't open the input file");
        return false;
    }

    std::string line;
    // read map name (name is currently ignored)
    size_t line_number = 1;
    if (!(std::getline(file, line)))
    {
        GameBoardInitializer::logInputError("Line 1: file does not contain the map name");
        return false;
    }

    // read lines 2-5 with the key-value pairs
    size_t max_steps, tanks_num_shells;
    size_t height, width;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "MaxSteps", max_steps))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "NumShells", tanks_num_shells))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Rows", height))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Cols", width))
        return false;

    setBoardDetails(width, height, max_steps, tanks_num_shells);

    // read the board itself
    size_t board_row_number = 0;

    while (board_row_number < height && std::getline(file, line))
    {

        if (!GameBoardInitializer::parseBoardLine(line, width, board_row_number, board, addObjectToBoard))
        {
            return false; // return if some error in board - should not happen
        }

        line_number++;      // advance the read line if read any;
        board_row_number++; // advance board row
    }

    if (board_row_number < height)
    {
        GameBoardInitializer::logInputError("Actual board did not contain enough rows - filled by empty space");
    }
    else if (std::getline(file, line))
    {
        GameBoardInitializer::logInputError("Actual board contains too many rows - ignored");
    }

    file.close();
    return true;
}