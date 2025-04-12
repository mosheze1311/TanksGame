#include "../Logger/Logger.h"
#include "GameBoard.h"


// Constructor Implementation

BoardCell::BoardCell() : x(0), y(0) {}
BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (x < other.x) || (x == other.x && y < other.y);
}
BoardCell BoardCell::operator+(Direction dir) const
{
    auto [dx, dy] = offset(dir);
    return BoardCell(x + dx, y + dy);
}
// TODO: Constructor: Reads game board details from a file

BoardCell BoardCell::operator-(Direction dir) const
{
    auto [dx, dy] = offset(dir);
    return BoardCell(x - dx, y - dy);
}


GameBoard::GameBoard(int height, int width){
    this->board_details.height = height;
    this->board_details.width = width;
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
std::unordered_set<GameObject*> GameBoard::objectOnCell(const BoardCell& c) const {
    auto it = board.find(c);
    if (it != board.end()) {
        return it->second;
    }
    return {}; // Return an empty set
}


void GameBoard::addObject(GameObject* obj, BoardCell c)
{
    GameObject *game_object = nullptr;

    switch (obj->getObjectType())
    {
    case GameObjectType::tank1:
        if (auto tank = dynamic_cast<Tank *>(obj))
        {
            this->board_details.p1_tanks++;
            game_object = new Tank(*tank);
        }
        break;

    case GameObjectType::tank2:
        if (auto tank = dynamic_cast<Tank *>(obj))
        {
            this->board_details.p2_tanks++;
            game_object = new Tank(*tank);
        }
        break;

    case GameObjectType::mine:
        if (auto mine = dynamic_cast<Mine *>(obj))
        {
            this->board_details.mines++;
            game_object = new Mine(*mine);
        }
        break;

    case GameObjectType::wall:
        if (auto wall = dynamic_cast<Wall *>(obj))
        {
            this->board_details.walls++;
            game_object = new Wall(*wall);
        }
        break;

    case GameObjectType::shell:
        if (auto shell = dynamic_cast<Shell *>(obj))
        {
            game_object = new Shell(*shell);
        }
        break;

    default:
        // Optional: handle unknown object types
        break;
    }

    c = this->createAdjustedBoardCell(c);
    this->addObjectInternal(game_object, c);
}

vector<GameObject*> GameBoard::getGameObjects(GameObjectType t) const {
    vector<GameObject*> res;
    for (pair<GameObject*, BoardCell> iter: this->objects_locations)
            {
                if (iter.first->getObjectType() == t)
                {
                    res.push_back(iter.first);
                }
            }
    return res;
}

void GameBoard::moveGameObject(GameObject* obj, BoardCell new_pos){
    
    // object not exist
    if (objects_locations.find(obj) == objects_locations.end())
    {
        return;
    }
    this->removeObjectInternal(obj);
    
    new_pos = this->createAdjustedBoardCell(new_pos);
    this->addObjectInternal(obj, new_pos);
}

void GameBoard::removeObjectInternal(GameObject *obj)
{
    BoardCell c = objects_locations[obj];
    this->objects_locations.erase(obj);
    this->board[c].erase(obj);
    
    if(board[c].empty()){
        board.erase(c);
    }
}

void GameBoard::addObjectInternal(GameObject *obj, BoardCell c){
    this->objects_locations[obj] = c;
    this->board[c].insert(obj);
}