#include "BoardFactory.h"

#include <fstream>
#include <sstream>
#include "../Logger/Logger.h"

using namespace std;

void BoardFactory::logInputError(const string error_message)
{
    Logger::input().logError(error_message);
}

GameObject *BoardFactory::createGameObjectOfType(GameBoard &board, GameObjectType type)
{
    switch (type)
    {
    case GameObjectType::tank1:
        return new Tank(board, type, Direction::LEFT);
    case GameObjectType::tank2:
        return new Tank(board, type, Direction::RIGHT);
    case GameObjectType::wall:
        return new Wall(board);
    case GameObjectType::mine:
        return new Mine(board);
    default:
        return nullptr; // Handle unknown type safely
    }
}

GameBoard* BoardFactory::createGameBoard(const string file_path)
{
    /*
    input file expected format:
    First line should contain the game board setting in the following order (separated only by spaces):
        height, width, tanks_p1, tanks_p2, walls, mines

    The following lines should contain the locations of the each object on the board, these lines should be formatted as:
        game_object_char x y.

    Position is 0-Indexed

    The order of the object types is not enforced

    Only first n objects of a each type will be used (where n is the decalred object type count from line 1)
        --If there are less than n objects listed, declared number will be overriden.

    Empty lines are Ignored

    Example (4(h)*5(w) board with 3 p1 tanks, 2 p2 tanks, 1 wall, 1 mines):
        4 5 3 2 1 1
        1 0 0
        1 0 1
        1 0 2

        2 1 1
        2 1 2

        # 2 2
        @ 3 3
    */

    ifstream file(file_path);
    if (!file)
    {
        BoardFactory::logInputError("Cant open the input file");
        return NULL; // Handle file open failure
    }

    string line;
    if (!(std::getline(file, line)))
    {
        BoardFactory::logInputError("File does not contain the first mandatory line");
        return NULL;
    };

    std::istringstream first_line_iss(line);
    int height, width;
    int p1_tanks, p2_tanks, walls, mines;
    if (!(first_line_iss >> height >> width >> p1_tanks >> p2_tanks >> walls >> mines))
    {
        BoardFactory::logInputError("Invalid first row format: '" + line + "'");
        return NULL;
    }

    GameBoard* board = new GameBoard(height, width);
    map<char, int> counters;
    counters[(char)GameObjectType::tank1] = p1_tanks;
    counters[(char)GameObjectType::tank2] = p2_tanks;
    counters[(char)GameObjectType::mine] = mines;
    counters[(char)GameObjectType::wall] = walls;

    char obj_type;
    int x, y;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        obj_type = line[0];
        if (counters.find(obj_type) == counters.end())
        {
            BoardFactory::logInputError("Object type " + std::string(1, static_cast<char>(obj_type)) + " is invalid");
            continue;
        }

        if (counters[obj_type] == 0)
        {
            BoardFactory::logInputError("Can't add another object of type '" + std::string(1, obj_type) + "'");
            continue;
        }

        GameObject *go = createGameObjectOfType(*board, (GameObjectType)obj_type);
        if (!go)
        {
            logInputError("Failed to create object for type '" + string(1, obj_type) + "'");
            continue;
        }
        
        std::istringstream iss(line.substr(1)); // skip the first char (obj_type)
        if (!(iss >> x >> y))
        {
            BoardFactory::logInputError("Invalid row format: '" + line + "'");
            continue;
        }

        if (x < 0 || y < 0 || x >= width || y >= height)
        {
            logInputError("Object position (" + to_string(x) + ", " + to_string(y) + ") is out of board bounds");
            continue;
        }

        BoardCell c(x, y);
        board->addObject(go, c);
        counters[obj_type]--;
    }
    file.close();
    return board;
}