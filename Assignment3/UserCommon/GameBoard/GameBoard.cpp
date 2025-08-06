#include "GameBoard.h"

#include "../GameObjects/GameObjects.h"
#include "../Utils/GameObjectTypeUtils.h"

namespace UserCommon_211388913_322330820
{

    // === Constructor === //
    GameBoard::GameBoard() : AbstractGameBoardView(0, 0, 0, 0) {}

    // === Init Board === //
    void GameBoard::initFromDetails(const SatelliteView &sat_view, size_t width, size_t height, size_t max_steps, size_t num_shells)

    {
        this->setWidth(width);
        this->setHeight(height);
        board_details.tanks_num_shells = num_shells;
        board_details.max_steps = max_steps;
        for (size_t x = 0; x < width; ++x)
        {
            for (size_t y = 0; y < height; ++y)
            {
                char object_char = sat_view.getObjectAt(x, y);
                if (GameObjectTypeUtils::isValidObjectChar(object_char))
                {
                    this->addObject(this->createGameObjectOfType(static_cast<GameObjectType>(object_char)),
                                    BoardCell(x, y));
                }
            }
        }
    };

    // === Private Functions === //
    std::unique_ptr<GameObject> GameBoard::createGameObjectOfType(GameObjectType type)
    {
        switch (type)
        {
        case GameObjectType::TANK1:
        case GameObjectType::TANK2:
            return std::make_unique<Tank>(
                *this,
                type,
                ConfigReader::getConfig().getSpawnDirections()[GameObjectTypeUtils::tankTypeToPlayerIndex(type) - 1],
                this->getTanksNumShells());

        case GameObjectType::WALL:
            return std::make_unique<Wall>(*this);

        case GameObjectType::MINE:
            return std::make_unique<Mine>(*this);

        case GameObjectType::SHELL:
            return std::make_unique<Shell>(*this, Direction::DOWN);
            // Should never happen, input file should not contain shells.

        default:
            return nullptr; // should not get here
        }
    }

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

    // === Getters === //
    std::unordered_set<GameObject *> GameBoard::getObjectsOnCell(const BoardCell &c) const
    {
        auto it = board.find(c);
        if (it != board.end())
        {
            return it->second;
        }
        return {};
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

    // === Override === //
    std::vector<BoardCell> GameBoard::getOccupiedCells() const
    {
        std::vector<BoardCell> occupied_cells;
        for (auto iter : this->board)
        {
            occupied_cells.push_back(iter.first);
        }

        return occupied_cells;
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

    // === Board State === //
    bool GameBoard::isOccupiedCell(const BoardCell &c) const
    {
        return board.find(c) != board.end();
    }

    bool GameBoard::isObjectOnBoard(const GameObject *obj) const
    {
        return this->objects_locations.find(const_cast<GameObject *>(obj)) != this->objects_locations.end();
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
}