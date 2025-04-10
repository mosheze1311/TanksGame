#include "../Logger/Logger.h"
#include "GameBoard.h"
GameObject *createGameObjectOfType(GameObjectType type, Direction dir = Direction::RIGHT)
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

// Constructor Implementation

BoardCell::BoardCell() : x(0), y(0) {}
BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (x < other.x) || (x == other.x && y < other.y);
}

// TODO: Constructor: Reads game board details from a file
GameBoard::GameBoard(const string &file_path)
{
    ifstream file(file_path);
    if (!file)
    {
        Logger::getInstance().logError("Cant open the input file");
        return; // Handle file open failure
    }

    file >> board_details.height >> board_details.width >> board_details.p1_tanks >> board_details.p2_tanks >> board_details.walls >> board_details.mines;

    // TODO: Read all game objects from the file
    int n = board_details.p1_tanks + board_details.p2_tanks + board_details.mines + board_details.walls;
    for (int i = 0; i < n; i++)
    {
        char obj_type;
        int x, y;
        file >> obj_type >> x >> y;
        cout << obj_type << endl;
        BoardCell c(x, y);
        GameObject* go = createGameObjectOfType((GameObjectType)obj_type);
        this->addObject(go, c);
    }

    file.close();
}

// Getters
int GameBoard::getWidth() const
{
    return board_details.width;
}
int GameBoard::getHeight() const
{
    return board_details.height;
}

// Check if a cell is occupied
bool GameBoard::isOccupiedCell(const BoardCell &c) const
{
    return board.find(c) != board.end();
}
// Get the object at a cell (returns pointer or NULL)
GameObject *GameBoard::objectOnCell(const BoardCell &c) const
{
    if (isOccupiedCell(c))
    {
        return (board.find(c))->second;
    }
    return nullptr;
}

void GameBoard::addObject(GameObject* obj, BoardCell c)
{
    board[c] = obj;
    objects_locations[obj] = c;
}

vector<GameObject*> GameBoard::getGameObjects(GameObjectType t) const {
    vector<GameObject*> res;
    for (pair<GameObject*, BoardCell> iter: this->objects_locations){
        if(iter.first->getObjectType() == t){
            res.push_back(iter.first);
        }
    }
    return res;
}

// void GameBoard::moveGameObject(GameObject* obj, BoardCell new_pos){
    
//     // object not exist
//     if (objects_locations.find(obj) == objects_locations.end())
//     {
//         return;
//     }
//     new_pos = this->createBoardCell(new_pos);
    
//     BoardCell old_pos = objects_locations[obj];
//     board.erase(old_pos);

//     objects_locations[obj] = new_pos;
//     board[new_pos] = obj;
// }
