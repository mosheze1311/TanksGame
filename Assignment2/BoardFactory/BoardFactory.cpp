#include "BoardFactory.h"
#include <fstream>
#include <sstream>
#include <memory>

//=== Input File Handling ===
void BoardFactory::logInputError(const string error_message)
{
    Logger::input().logLine(error_message);
}

bool BoardFactory::parseKeyLine(const string &line, size_t line_number, const string &expected_key, size_t &out_value)
{
    istringstream iss(line);
    string key, equal_sign;
    size_t value;

    if (!(iss >> key))
    {
        BoardFactory::logInputError("Line " + std::to_string(line_number) + ": Missing key.\n");
        return false;
    }
    if (key != expected_key)
    {
        BoardFactory::logInputError("Line " + std::to_string(line_number) + ": Unexpected key '" + key + "', expected '" + expected_key + "'.\n");
        return false;
    }
    if (!(iss >> equal_sign) || equal_sign != "=")
    {
        BoardFactory::logInputError("Line " + std::to_string(line_number) + ": Expected '=' after key.");
        return false;
    }

    if (!(iss >> value))
    {
        BoardFactory::logInputError("Line " + std::to_string(line_number) + ": Expected numeric value after '='.");
        return false;
    }

    out_value = value;
    return true;
}

bool BoardFactory::parseBoardLine(GameBoard &board, const string &line, size_t line_number, size_t cols)
{
    cols = min(cols, line.length()); // ensure no overflowing lines.

    char obj_char;
    for (size_t col = 0; col < cols; col++)
    {
        obj_char = line[col];
        if (!GameObjectTypeUtils::isValidObjectChar(obj_char))
            continue;

        BoardFactory::addObjectToBoard(board, obj_char, col, line_number);
    }

    return true;
}

unique_ptr<GameObject> BoardFactory::createGameObjectOfType(GameBoard &board, GameObjectType type)
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
            GameObjectTypeUtils::tankTypeToPlayerIndex(type) % 2 == 1 ? Direction::LEFT : Direction::RIGHT,
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

bool BoardFactory::initGameBoardFromFile(GameBoard &board, const string input_file_path)
{
    /*
    return: true iff could initiate board using the input file.

    input file expected format:

    */

    ifstream file(input_file_path);
    // Handle file open failure
    if (!file)
    {
        BoardFactory::logInputError("Can't open the input file");
        return false;
    }

    string line;
    // read map name (name is currently ignored)
    size_t line_number = 1;
    if (!(std::getline(file, line)))
    {
        BoardFactory::logInputError("Line 1: file does not contain the map name");
        return false;
    };

    // read lines 2-5 with the key-value pairs
    size_t max_steps, tanks_num_shells;
    size_t rows, cols;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "MaxSteps", max_steps))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "NumShells", tanks_num_shells))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Rows", rows))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Cols", cols))
        return false;

    BoardFactory::setBoardDimensions(board, rows, cols);
    BoardFactory::setBoardDetails(board, max_steps, tanks_num_shells);

    // read the board itself
    size_t board_row_number = 0;

    while (board_row_number < rows && std::getline(file, line))
    {
        line_number++;      // advance the read line if read any;
        board_row_number++; // advance board row

        if (!BoardFactory::parseBoardLine(board, line, board_row_number, cols))
        {
            return false; // return if some error in board - should not happen
        }
    }

    return true;
}

//=== Board Manipulations ===
void BoardFactory::setBoardDimensions(GameBoard &board, size_t rows, size_t cols)
{
    board.setWidth(cols);
    board.setHeight(rows);
}

void BoardFactory::setBoardDetails(GameBoard &board, size_t max_steps, size_t tanks_num_shells)
{
    board.setMaxSteps(max_steps);
    board.setTanksNumShells(tanks_num_shells);
}

void BoardFactory::addObjectToBoard(GameBoard &board, const char object_char, int x, int y)
{
    unique_ptr<GameObject> ptr = createGameObjectOfType(board, (GameObjectType)object_char);
    board.addObject(std::move(ptr), {x, y});
}
