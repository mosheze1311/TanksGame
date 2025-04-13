#include "GameBoard.h"

#include "../GameObjects/GameObjects.h"
#include "../Logger/Logger.h"
#include <iostream>

using namespace std;

// === Constructor ===
GameBoard::GameBoard(int height, int width): board_details(height, width){}
GameBoard::GameBoard(GameBoard &other) : board_details(other.board_details)
{
    for (auto iter : other.objects_locations)
    {
        this->addObjectInternal(iter.first,iter.second);
    }
}
// === Board Info Getters ===
int GameBoard::getWidth() const
{
    return board_details.width;
}

int GameBoard::getHeight() const
{
    return board_details.height;
}

// === Board State ===
bool GameBoard::isOccupiedCell(const BoardCell &c) const
{
    return board.find(c) != board.end();
}

vector<BoardCell> GameBoard::getOccupiedCells() const{
    vector<BoardCell> occupied_cells;
    for (auto iter : this->board)
    {
        occupied_cells.push_back(iter.first);
    }

    return occupied_cells;
}

BoardCell GameBoard::createBoardCell(int x, int y)
{
    return BoardCell(
        (x + this->board_details.width) % this->board_details.width,
        (y + this->board_details.height) % this->board_details.height);
}

BoardCell GameBoard::createAdjustedBoardCell(const BoardCell &c)
{
    return this->createBoardCell(c.getX(), c.getY());
}

std::unordered_set<GameObject *> GameBoard::getObjectsOnCell(const BoardCell &c) const
{
    auto it = board.find(c);
    if (it != board.end())
    {
        return it->second;
    }
    return {};
}

optional<BoardCell> GameBoard::getObjectLocation(GameObject *obj) const
{
    auto iter = this->objects_locations.find(obj);
    if (iter == this->objects_locations.end())
    {
        return nullopt;
    }
    return iter->second;
}

// === Object Management ===
bool GameBoard::isObjectOnBoard(GameObject* obj) const{
    return this->objects_locations.find(obj) != this->objects_locations.end();
}

void GameBoard::addObject(GameObject *obj, BoardCell c)
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
        break;
    }

    c = this->createAdjustedBoardCell(c);
    this->addObjectInternal(game_object, c);
}

void GameBoard:: removeObject(GameObject* obj){
    if(!isObjectOnBoard(obj)){
        return;
    }
    this->removeObjectInternal(obj);
}

vector<GameObject *> GameBoard::getGameObjects(GameObjectType t) const
{
    vector<GameObject *> res;
    for (const pair<GameObject *, BoardCell> iter : this->objects_locations)
    {
        if (iter.first->getObjectType() == t)
        {
            res.push_back(iter.first);
        }
    }
    return res;
}

vector<GameObject *> GameBoard::getAllGameObjects(){
    vector<GameObject *> res;
    for (const pair<GameObject *, BoardCell> iter : this->objects_locations)
    {
        res.push_back(iter.first);
    }
    return res;
}

void GameBoard::moveGameObject(GameObject *obj, BoardCell new_pos)
{
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

    if (board[c].empty())
    {
        board.erase(c);
    }
}

void GameBoard::addObjectInternal(GameObject *obj, BoardCell c)
{
    this->objects_locations[obj] = c;
    this->board[c].insert(obj);
}

// === Game Logic ===
void GameBoard::moveTanksRandomly()
{
    vector<GameObject *> tanks = this->getGameObjects(GameObjectType::tank1);
    vector<GameObject *> tank2 = this->getGameObjects(GameObjectType::tank2);

    tanks.insert(tanks.end(), tank2.begin(), tank2.end());

    for (GameObject *tank : tanks)
    {
        optional<BoardCell> opt_cell = this->getObjectLocation(tank);
        if (!opt_cell)
        {
            continue;
        }

        BoardCell tank_location = *opt_cell;

        bool changeX = std::rand() % 2;
        int delta = (std::rand() % 2 == 0) ? -1 : 1;

        int newX = tank_location.getX();
        int newY = tank_location.getY();

        if (changeX)
        {
            newX += delta;
        }
        else
        {
            newY += delta;
        }

        BoardCell new_cell = this->createBoardCell(newX, newY);
        moveGameObject(tank, new_cell);
    }
}