#include "GameBoard.h"

#include "../GameObjects/GameObjects.h"
#include "../Utils/GameBoardUtils.h"

// === BoardDetails Constructor === //
GameBoard::BoardDetails::BoardDetails(int height, int width) : height(height), width(width),
                                                               max_steps(0), tanks_num_shells(0),
                                                               walls(0), mines(0), shells(0), tanks_count(),
                                                               remaining_shells(0) {};

// === Constructor === //
GameBoard::GameBoard(int height, int width) : board_details(height, width) {}

// === Private Functions === //
void GameBoard::removeObjectInternal(GameObject *obj, bool final)
{
    BoardCell c = objects_locations[obj];
    this->objects_locations.erase(obj);
    this->board[c].erase(obj);

    if (board[c].empty())
    {
        board.erase(c);
    }

    // Remove from owned_objects
    if (final)
        owned_objects.erase(obj);
}

void GameBoard::addObjectInternal(std::unique_ptr<GameObject> obj, const BoardCell &c)
{
    GameObject *raw_ptr = obj.get();
    this->owned_objects[raw_ptr] = std::move(obj);

    this->placeObjectOnBoard(raw_ptr, c);
}

void GameBoard::addObject(std::unique_ptr<GameObject> obj, const BoardCell &c)
{
    this->updateObjectCount(obj.get(), 1);
    this->addObjectInternal(std::move(obj), this->createAdjustedBoardCell(c));
}

void GameBoard::placeObjectOnBoard(GameObject *obj, const BoardCell &c)
{
    this->objects_locations[obj] = c;
    this->board[c].insert(obj);
}

void GameBoard::updateObjectCount(const GameObject *obj, int incremental = 1)
{
    // incremental can only be 1 or -1.
    switch (obj->getObjectType())
    {
    case GameObjectType::TANK1:
    case GameObjectType::TANK2:
    case GameObjectType::TANK3:
    case GameObjectType::TANK4:
    case GameObjectType::TANK5:
    case GameObjectType::TANK6:
    case GameObjectType::TANK7:
    case GameObjectType::TANK8:
    case GameObjectType::TANK9:
        this->board_details.tanks_count[obj->getObjectType()] += incremental;
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

void GameBoard::useTankShell()
{
    this->board_details.remaining_shells--;
}

BoardCell GameBoard::createBoardCell(int x, int y) const
{
    return this->createAdjustedBoardCell(BoardCell(x,y));
}

BoardCell GameBoard::createAdjustedBoardCell(const BoardCell &c) const
{
    return GameBoardUtils::createAdjustedBoardCell(c, this->getWidth(), this->getHeight());
}

// === Init from File === //
bool GameBoard::initFromFile(const std::string &input_file_path)
{
    auto setBoardDetails = [this](int width, int height, int max_steps, int tanks_num_shells)
    {
        this->setWidth(width);
        this->setHeight(height);
        this->setMaxSteps(max_steps);
        this->setTanksNumShells(tanks_num_shells);
    };
    auto addObject = [this](std::unique_ptr<GameObject> ptr, const BoardCell &cell)
    {
        this->addObject(std::move(ptr), cell);
    };

    return GameBoardInitializer::initGameBoardFromFile(input_file_path, *this, setBoardDetails, addObject);
}

// === Getters === //
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

std::map<GameObjectType, size_t> GameBoard::getTanksCountPerType() const
{
    return this->board_details.tanks_count;
}

int GameBoard::getGameObjectCount(const GameObjectType type) const
{
    switch (type)
    {
    case GameObjectType::TANK1:
    case GameObjectType::TANK2:
    case GameObjectType::TANK3:
    case GameObjectType::TANK4:
    case GameObjectType::TANK5:
    case GameObjectType::TANK6:
    case GameObjectType::TANK7:
    case GameObjectType::TANK8:
    case GameObjectType::TANK9:
        try
        {
            return this->board_details.tanks_count.at(type);
        }
        catch (const std::out_of_range &e)
        {
            return 0;
        }
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

std::vector<BoardCell> GameBoard::getOccupiedCells() const
{
    std::vector<BoardCell> occupied_cells;
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

std::unordered_set<GameObjectType> GameBoard::getObjectsTypesOnCell(const BoardCell &c) const
{
    std::unordered_set<GameObjectType> res;
    for (GameObject *go : this->getObjectsOnCell(c))
    {
        res.insert(go->getObjectType());
    }
    return res;
}

std::optional<BoardCell> GameBoard::getObjectLocation(const GameObject *obj) const
{
    auto iter = this->objects_locations.find(const_cast<GameObject *>(obj));
    if (iter == this->objects_locations.end())
    {
        return std::nullopt;
    }
    return iter->second;
}

std::vector<GameObject *> GameBoard::getGameObjects(GameObjectType t) const
{
    // Returns a vector of all instances if a specific GameObject
    std::vector<GameObject *> res;
    for (const auto &iter : this->owned_objects)
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
    if (!GameObjectTypeUtils::isTankObject(t))
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

std::vector<GameObject *> GameBoard::getAllGameObjects() const
{
    std::vector<GameObject *> res;
    for (const auto &obj : this->owned_objects)
    {
        res.push_back(obj.first);
    }
    return res;
}

std::vector<Tank *> GameBoard::getAllTanksOrderedByCell() const
{
    std::vector<Tank *> res;
    for (auto [cell, objects_set] : this->board)
    {
        for (GameObject *go : objects_set)
        {
            if (GameObjectTypeUtils::isTankObject(go->getObjectType()))
            {
                res.push_back(static_cast<Tank *>(go));
            }
        }
    }
    return res;
}

// === Board State === //
bool GameBoard::isOccupiedCell(const BoardCell &c) const
{
    return board.find(c) != board.end();
}

bool GameBoard::isObjectOnBoard(const GameObject *obj) const
{
    // TODO: const cast
    return this->objects_locations.find(const_cast<GameObject *>(obj)) != this->objects_locations.end();
}

// === Setters === //
void GameBoard::setWidth(size_t width)
{
    this->board_details.width = width;
}

void GameBoard::setHeight(size_t height)
{
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

// === Modify Board Functions === //
void GameBoard::removeObject(GameObject *obj)
{
    if (!isObjectOnBoard(obj))
    {
        return;
    }

    this->updateObjectCount(obj, -1);
    this->removeObjectInternal(obj);
}

void GameBoard::moveGameObject(GameObject *obj, const BoardCell &new_pos)
{
    if (this->objects_locations.find(obj) == this->objects_locations.end())
    {
        return;
    }

    this->removeObjectInternal(obj, false);
    this->placeObjectOnBoard(obj, this->createAdjustedBoardCell(new_pos));
}

void GameBoard::addTankShell(std::unique_ptr<Shell> shell, const BoardCell &shell_initial_cell)
{
    this->addObject(std::move(shell), shell_initial_cell);
    this->useTankShell();
}