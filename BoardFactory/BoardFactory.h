#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <fstream>

#include <string>

using namespace std;

// TODO: log errors into input_errors.txt
class BoardFactory
{
private:    
    GameObject *createGameObjectOfType(GameObjectType type, Direction dir = Direction::RIGHT);
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

GameObject * BoardFactory:: createGameObjectOfType(GameObjectType type, Direction dir = Direction::RIGHT)
{
    switch (type)
    {
    case GameObjectType::tank1:
        return new Tank(type, Direction::LEFT);
    case GameObjectType::tank2:
        return new Tank(type, Direction::RIGHT);
    case GameObjectType::wall:
        return new Wall();
    case GameObjectType::mine:
        return new Mine();
    case GameObjectType::shell:
        return new Shell(dir);
    default:
        return nullptr; // Handle unknown type safely
    }
}

GameBoard BoardFactory::createGameBoard(const string file_path){
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

    char obj_type;
    int x, y;
    string line;
    for (int i = 0; i < p1_tanks; i++)
    {
        file >> obj_type;
        if (obj_type != (char)GameObjectType::tank1)
        {
            
        }
    }
    // TODO: Read all game objects from the file
    int n = p1_tanks + p2_tanks + mines + walls;
    for (int i = 0; i < n; i++)
    {

        file >> obj_type >> x >> y;
        cout << obj_type << endl;
        BoardCell c(x, y);
        GameObject *go = createGameObjectOfType((GameObjectType)obj_type);
        board.addObject(go, c);
    }

    file.close();

    return board;
}
