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
std::vector<GameObject*> GameBoard::objectOnCell(const BoardCell& c) const {
    auto it = board.find(c);
    if (it != board.end()) {
        return it->second;
    }
    return {}; // Return an empty vector
}


void GameBoard::addObject(GameObject* obj, BoardCell c)
{
    GameObject* game_object;
    switch (obj->getObjectType())
    {
    case GameObjectType::tank1:
        this->board_details.p1_tanks++;
        game_object = static_cast<Tank *>(obj);
        break;

    case GameObjectType::tank2:
        this->board_details.p2_tanks++;
        game_object = static_cast<Tank *>(obj);

        break;

    case GameObjectType::mine:
        this->board_details.mines++;
        game_object = static_cast<Mine *>(obj);

        break;

    case GameObjectType::wall:
        this->board_details.walls++;
        game_object = static_cast<Wall *>(obj);
        break;

    case GameObjectType::shell:
        game_object = static_cast<Shell *>(obj);
        break;
    default:
        break;
    }

    board[c].push_back(game_object);
    objects_locations[game_object] = c;
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
    new_pos = this->createBoardCell(new_pos);
    
    BoardCell old_pos = objects_locations[obj];
    board.erase(old_pos);

    objects_locations[obj] = new_pos;
    board[new_pos] = obj;
}
