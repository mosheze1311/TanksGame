#include "GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <queue>
#include <set>

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
    return this->board_details.width;
}

int GameBoard::getHeight() const
{
    return this->board_details.height;
}

int GameBoard::getGameObjectCount(GameObjectType type) const
{
    switch (type)
    {
    case GameObjectType::tank1:
        return this->board_details.p1_tanks;
    case GameObjectType::tank2:
        return this->board_details.p2_tanks;
    case GameObjectType::wall:
        return this->board_details.walls;
    case GameObjectType::shell:
        return this->board_details.shells;
    case GameObjectType::mine:
        return this->board_details.mines;

    default: // should not get here
        return 0;
    }
}

int GameBoard::getTotalRemainingShells() const {
    return this->board_details.remaining_shells;
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

BoardCell GameBoard::createBoardCell(int x, int y) const
{
    return BoardCell(
        (x + this->board_details.width) % this->board_details.width,
        (y + this->board_details.height) % this->board_details.height);
}

BoardCell GameBoard::createAdjustedBoardCell(const BoardCell &c) const
{
    return this->createBoardCell(c.getX(), c.getY());
}

BoardCell GameBoard::getcreatedAdjustedBoardCell(const BoardCell &c) const
{
    return createAdjustedBoardCell(c);
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
    this->updateObjectCount(obj, 1);
    c = this->createAdjustedBoardCell(c);
    this->addObjectInternal(obj, c);
}

void GameBoard:: removeObject(GameObject* obj){
    if(!isObjectOnBoard(obj)){
        return;
    }

    this->updateObjectCount(obj, -1);
    this->removeObjectInternal(obj);
}

vector<GameObject *> GameBoard::getGameObjects(GameObjectType t) const
{
    // Returns a vector of all instances if a specific GameObject
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

// TODO: i think it sholud be in game manager, needs to check
std::vector<Tank*> GameBoard::getTanks(GameObjectType t) const {
    if (t != GameObjectType::tank1 && t!=GameObjectType::tank2){
        return {};
    }
    
    std::vector<GameObject*> objects = this->getGameObjects(t);
    std::vector<Tank*> tanks;
    tanks.reserve(objects.size()); // optional optimization

    for (GameObject* obj : objects) {
        if (Tank* tank = dynamic_cast<Tank*>(obj)) {
            tanks.push_back(tank);
        }
    }

    return tanks;
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

void GameBoard::updateObjectCount(GameObject* obj, int incremental = 1){
    // incremental can only be 1 or -1.
    switch (obj->getObjectType())
    {
    case GameObjectType::tank1:
            this->board_details.p1_tanks += incremental;
            this->board_details.remaining_shells += incremental * ((Tank *)obj)->getShells();

            break;

    case GameObjectType::tank2:
            this->board_details.p2_tanks += incremental;
            this->board_details.remaining_shells += incremental * ((Tank *)obj)->getShells();

            break;

    case GameObjectType::mine:
        this->board_details.mines += incremental;
        break;

    case GameObjectType::wall:
            this->board_details.walls+=incremental;
        break;

    case GameObjectType::shell:
            this->board_details.shells += incremental;
        break;

    default:
        break;
    }
}

void GameBoard::useTankShell(){
    this->board_details.remaining_shells--;
}

int GameBoard::distance(BoardCell first, BoardCell second) const{

    int dx = abs(first.getX() - second.getX());
    int dy = abs(first.getY() - second.getY());

    dx = std::min(dx, this->getWidth() - dx);
    dy = std::min(dy, this->getHeight() - dy);

    return std::max(dx, dy);
}

vector<BoardCell> GameBoard::getAdjacentCells(const BoardCell& curr_cell) const 
{
    vector<BoardCell> res;

    for (int dir_number = 0; dir_number < 8; dir_number++) {
        Direction dir = static_cast<Direction>(dir_number);
        BoardCell neighbor = this->createAdjustedBoardCell(curr_cell + dir);
        res.push_back(neighbor);
    }

    return res;
}

