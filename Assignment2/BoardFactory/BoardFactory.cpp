#include "BoardFactory.h"
#include <fstream>
#include <sstream>
#include <memory>


using namespace std;

//=== Input File Handling ===
void BoardFactory::logInputError(const string error_message)
{
    Logger::input().logError(error_message);
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

bool BoardFactory::parseBoardLine(GameBoard& board, const string &line, size_t line_number, size_t cols)
{
    cols = min(cols, line.length()); // ensure no overflowing lines.

    char obj_char;
    for (size_t col = 0; col < cols; col++)
    {
        obj_char = line[col];
        if (BoardFactory::isValidObjectChar(obj_char))
            continue;

        BoardFactory::addObjectToBoard(board, obj_char, col, line_number);
    }

    return true;
}

bool BoardFactory::isValidObjectChar(char obj_char) 
{
    // TODO: maybe move to GameObjectType enum class as helper function
    set<char> valid_chars({'1', '2', '#', '@'});

    return (valid_chars.find(obj_char) != valid_chars.end());
}

shared_ptr<GameObject> BoardFactory::createGameObjectOfType(GameBoard &board, GameObjectType type)
{
    switch(type)
        {
            case GameObjectType::TANK1:
                return std::make_shared<Tank>(board, type, Direction::LEFT);

            case GameObjectType::TANK2:
                return std::make_shared<Tank>(board, type, Direction::RIGHT);
            
            case GameObjectType::WALL:
                return std::make_shared<Wall>(board);
            
            case GameObjectType::MINE:
                return std::make_shared<Mine>(board);
            
            default:
              //  logInputError("Invalid object type: " + std::to_string((int)type));
                return nullptr;
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

    // TODO: This should be done in the GameBoard , but we need to set it here to be able to use the BoardFactory::addObjectToBoard function.
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
void BoardFactory::setBoardDimensions(GameBoard& board, size_t rows, size_t cols)
{
    board.setWidth(cols);
    board.setHeight(rows);
}

void BoardFactory::setBoardDetails(GameBoard& board, size_t max_steps, size_t tanks_num_shells)
{
    board.setMaxSteps(max_steps);
    board.setTanksNumShells(tanks_num_shells);
}

void BoardFactory::addObjectToBoard(GameBoard& board, const char object_char, size_t x, size_t y)
{
    // TODO: fix this and match to GameBoard - currently not correct and gives char instead of GameObject*
    board.addObject(createGameObjectOfType((GameObjectType)object_char), {static_cast<int>(x), static_cast<int>(y)});
}
