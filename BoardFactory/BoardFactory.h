#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// TODO: log errors into input_errors.txt
class BoardFactory
{
private:
    GameObject *createGameObjectOfType(GameBoard &board, GameObjectType type);
    void logInputError();

public:
    BoardFactory(/* args */);
    ~BoardFactory();

    GameBoard createGameBoard(const string file_path);
};

BoardFactory::BoardFactory(/* args */)
{
}

BoardFactory::~BoardFactory()
{
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

GameBoard BoardFactory::createGameBoard(const string file_path)
{
    ifstream file(file_path);
    if (!file)
    {
        Logger::getInstance().logError("Cant open the input file");
        return; // Handle file open failure
    }

    int height, width;
    int p1_tanks, p2_tanks, walls, mines;
    file >> height >> width;
    file >> p1_tanks >> p2_tanks >> walls >> mines;

    GameBoard board(height, width);
    map<char, int> counters;
    counters[(char)GameObjectType::tank1] = p1_tanks;
    counters[(char)GameObjectType::tank2] = p2_tanks;
    counters[(char)GameObjectType::mine] = mines;
    counters[(char)GameObjectType::wall] = walls;

    char obj_type;
    int x, y;

    string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        char obj_type = line[0]; // assume first character represents the type
        if (counters.find(obj_type) == counters.end())
        {
            Logger::getInstance().logError("obj type is inncorrect");
            continue;
        }
        if (counters[obj_type] == 0)
        {
            Logger::getInstance().logError("cant add another object of type '" + std::string(1, static_cast<char>(obj_type)) + "'");
            continue;
        }

        std::istringstream iss(line.substr(1)); // skip the first char (obj_type)
        int x,y;
        if (!(iss >> x >> y))
        {
            Logger::getInstance().logError("Invalid coordinates format: '" + line + "'");

            continue;
        }

        BoardCell c(x, y);
        GameObject *go = createGameObjectOfType(board, (GameObjectType)obj_type);
        board.addObject(go, c);
        counters[obj_type]--;
    }
    file.close();

    return board;
}
