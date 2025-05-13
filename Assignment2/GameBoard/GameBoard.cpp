#include "GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../Logger/Logger.h"
#include <queue>
#include <set>

using namespace std;

// === Constructors ===
GameBoard::GameBoard(int height, int width) : board_details(height, width) {}

GameBoard::~GameBoard()
{
    // TODO: make sure deletion is good (maybe not freeing?)
    auto locs = this->objects_locations;
    for (auto iter : locs)
    {
        this->removeObjectInternal(iter.first);
    }
}

// === Private Functions ===
void GameBoard::removeObjectInternal(GameObject* obj, bool final)
{
    BoardCell c = objects_locations[obj];
    this->objects_locations.erase(obj);
    this->board[c].erase(obj);

    if (board[c].empty())
    {
        board.erase(c);
    }
    
    // Remove from owned_objects
    if(final)
        owned_objects.erase(obj);
}

void GameBoard::addObjectInternal(unique_ptr<GameObject> obj, const BoardCell &c)
{
    GameObject *raw_ptr = obj.get();
    this->owned_objects[raw_ptr] = std::move(obj);

    this->placeObjectOnBoard(raw_ptr, c);
}

void GameBoard::placeObjectOnBoard(GameObject* obj, const BoardCell& c){
    this->objects_locations[obj] = c;
    this->board[c].insert(obj);
}

void GameBoard::updateObjectCount(const GameObject* obj, int incremental = 1)
{
    // incremental can only be 1 or -1.
    switch (obj->getObjectType())
    {
    case GameObjectType::TANK1:
        this->board_details.p1_tanks += incremental;
        this->board_details.remaining_shells += incremental * (static_cast<const Tank*>(obj))->getShells();
        break;

    case GameObjectType::TANK2:
        this->board_details.p2_tanks += incremental;
        this->board_details.remaining_shells += incremental * (static_cast<const Tank *>(obj))->getShells();
        break;

    case GameObjectType::MINE:
        this->board_details.mines += incremental;
        break;

    case GameObjectType::WALL:
        this->board_details.walls += incremental;
        break;

    case GameObjectType::SHELL:
        this->board_details.shells += incremental;
        break;

    default:
        break;
    }
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

// === Getters ===
size_t GameBoard::getWidth() const
{
    return this->board_details.width;
}

size_t GameBoard::getHeight() const
{
    return this->board_details.height;
}

size_t GameBoard::getMaxSteps() const
{
    return this->board_details.max_steps;
}

size_t GameBoard::getTanksNumShells() const
{
    return this->board_details.tanks_num_shells;
}

int GameBoard::getGameObjectCount(const GameObjectType type) const
{
    switch (type)
    {
    case GameObjectType::TANK1:
        return this->board_details.p1_tanks;
    case GameObjectType::TANK2:
        return this->board_details.p2_tanks;
    case GameObjectType::WALL:
        return this->board_details.walls;
    case GameObjectType::SHELL:
        return this->board_details.shells;
    case GameObjectType::MINE:
        return this->board_details.mines;

    default: // should not get here
        return 0;
    }
}

int GameBoard::getTotalRemainingShells() const
{
    return this->board_details.remaining_shells;
}

vector<BoardCell> GameBoard::getOccupiedCells() const
{
    vector<BoardCell> occupied_cells;
    for (auto iter : this->board)
    {
        occupied_cells.push_back(iter.first);
    }

    return occupied_cells;
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

optional<BoardCell> GameBoard::getObjectLocation(const GameObject *obj) const
{
    auto iter = this->objects_locations.find(const_cast<GameObject*>(obj));
    if (iter == this->objects_locations.end()) {
        return nullopt;
    }
    return iter->second;
}

vector<GameObject *> GameBoard::getGameObjects(GameObjectType t) const
{
    // Returns a vector of all instances if a specific GameObject
    vector<GameObject *> res;
    for (const auto& iter : this->owned_objects)
    {
        if (iter.first->getObjectType() == t)
        {
            res.push_back(iter.first);
        }
    }
    return res;
}

std::vector<Tank *> GameBoard::getTanks(GameObjectType t) const
{
    if (t != GameObjectType::TANK1 && t != GameObjectType::TANK2)
        return {};
    

    std::vector<GameObject *> objects = this->getGameObjects(t);
    std::vector<Tank *> tanks;
    tanks.reserve(objects.size()); // optional optimization

    for (auto obj : objects)
    {
        if (Tank *tank = dynamic_cast<Tank *>(obj))
        {
            tanks.push_back(tank);
        }
    }

    return tanks;
}

vector<GameObject *> GameBoard::getAllGameObjects() const
{
    vector<GameObject *> res;
    for (const auto& obj : this->owned_objects)
    {
        res.push_back(obj.first);
    }
    return res;
}

vector<BoardCell> GameBoard::getAdjacentCells(const BoardCell &curr_cell) const
{
    vector<BoardCell> res;

    for (int dir_number = 0; dir_number < 8; dir_number++)
    {
        Direction dir = static_cast<Direction>(dir_number);
        BoardCell neighbor = this->createAdjustedBoardCell(curr_cell + dir);
        res.push_back(neighbor);
    }

    return res;
}

BoardCell GameBoard::getNextCellInDirection(const BoardCell &c, const Direction dir) const
{
    return this->createAdjustedBoardCell(c + dir);
}

// === Board State ===
bool GameBoard::isOccupiedCell(const BoardCell &c) const
{
    return board.find(c) != board.end();
}

bool GameBoard::isObjectOnBoard(const GameObject *obj) const
{
    return this->objects_locations.find(const_cast<GameObject*>(obj)) != this->objects_locations.end();
}

int GameBoard::xDistance(const BoardCell &first, const BoardCell &second) const
{
    size_t dx = abs(first.getX() - second.getX());
    dx = min(dx, this->getWidth() - dx);
    return dx;
}

int GameBoard::yDistance(const BoardCell &first, const BoardCell &second) const
{
    size_t dy = abs(first.getY() - second.getY());
    dy = std::min(dy, this->getHeight() - dy);
    return dy;
}

int GameBoard::distance(const BoardCell &first, const BoardCell &second) const
{
    return std::max(xDistance(first, second), yDistance(first, second));
}

bool GameBoard::isStraightLine(BoardCell from, BoardCell to) const
{
    int x_diff = xDistance(from, to);
    int y_diff = yDistance(from, to);

    if (x_diff == 0 || y_diff == 0)
    {
        return true;
    }
    if (abs(x_diff) == abs(y_diff))
    {
        return true;
    }

    return false;
}

BoardCell GameBoard::getNextCellInStraightLine(BoardCell from, BoardCell to) const
{
    // assuming that from, to form a straight line in some direction.
    // TODO: try to write it in a algebraic form
    // int normal_dx = to.getX() - from.getX();
    // int normal_dy = to.getY() - from.getY();

    // int vertical_border_dx = this->getWidth() + min(from.getX(), to.getX()) - max(from.getX(), to.getX());
    // int horizontal_border_dy = this->getHeight() + min(from.getY(), to.getY()) - max(from.getY(), to.getY());

    vector<BoardCell> neighbors = getAdjacentCells(from);
    int min_dist = distance(from, to);
    BoardCell next = from;

    for (BoardCell neighbor : neighbors)
    {
        int neighbor_dist = distance(neighbor, to);
        if (neighbor_dist < min_dist && isStraightLine(neighbor, to))
        {
            min_dist = neighbor_dist;
            next = neighbor;
        }
    }
    return next;
}

bool GameBoard::isDirectionMatch(BoardCell from, BoardCell to, Direction dir) const
{
    return this->getNextCellInDirection(from, dir) == this->getNextCellInStraightLine(from, to);
}

//=== Setters ===
void GameBoard::setWidth(size_t width)
{
    // TODO: maybe modify location to fit new board? maybe prevent using this after insertion to board.
    this->board_details.width = width;
}

void GameBoard::setHeight(size_t height)
{
    // TODO: maybe modify location to fit new board? maybe prevent using this after insertion to board.
    this->board_details.height = height;
}

void GameBoard::setMaxSteps(size_t max_steps)
{
    this->board_details.max_steps = max_steps;
}

void GameBoard::setTanksNumShells(size_t tanks_num_shells)
{
    this->board_details.tanks_num_shells = tanks_num_shells;
}

//=== Modify Board Functions ===
void GameBoard::addObject(unique_ptr<GameObject> obj, const BoardCell &c)
{
    this->updateObjectCount(obj.get(), 1);
    this->addObjectInternal(std::move(obj), this->createAdjustedBoardCell(c));
}

void GameBoard::removeObject(GameObject* obj)
{
    if (!isObjectOnBoard(obj))
    {
        return;
    }

    this->updateObjectCount(obj, -1);
    this->removeObjectInternal(obj);
}

void GameBoard::moveGameObject(GameObject* obj, const BoardCell &new_pos)
{
    if (this->objects_locations.find(obj) == this->objects_locations.end())
    {
        return;
    }

    this->removeObjectInternal(obj, false);
    this->placeObjectOnBoard(obj, this->createAdjustedBoardCell(new_pos));
}

void GameBoard::useTankShell()
{
    this->board_details.remaining_shells--;
}
