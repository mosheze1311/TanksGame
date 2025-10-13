#include "../../UserCommon/Config/ConfigReader.h"

#include "SatelliteAnalyticsView.h"

namespace Algorithm_211388913_322330820
{
    using namespace DirectionUtils; // for multiplying direction by a const

    // === Constructor === //
    SatelliteAnalyticsView::SatelliteAnalyticsView(size_t height, size_t width, size_t max_steps_gap, int player_idx)
        : AbstractGameBoardView(width, height, 0, 0), player_idx(player_idx), max_steps_gap(max_steps_gap) {};

    // === Private Functions === //
    // === Internal Logic === //
    std::vector<std::pair<char, AssumedDirection>> SatelliteAnalyticsView::getObjectsAtInternal(const BoardCell &c) const
    {
        auto iter = this->analytical_view.find(c);
        if (iter == this->analytical_view.end())
            return {};

        std::vector<std::pair<char, AssumedDirection>> objects_pairs;
        for (auto pair : iter->second)
        {
            objects_pairs.push_back({static_cast<char>(pair.first), pair.second});
        }

        return objects_pairs;
    }

    void SatelliteAnalyticsView::addObject(const BoardCell &location, GameObjectType t, AssumedDirection d)
    {
        addObjectInternal(this->createAdjustedBoardCell(location), t, d);
    }

    void SatelliteAnalyticsView::addObjectInternal(const BoardCell &location, GameObjectType obj_type, AssumedDirection dir)
    {
        // add to map
        this->analytical_view[location].push_back({obj_type, dir});

        // add to type sets
        if (obj_type == GameObjectType::SHELL)
            shells_locations.insert(location);

        else if (obj_type == GameObjectType::MINE)
            mines_locations.insert(location);

        else if (GameObjectTypeUtils::isTankObject(obj_type) &&
                 GameObjectTypeUtils::tankTypeToPlayerIndex(obj_type) != this->player_idx)
            enemies_locations.insert(location);

        else if (GameObjectTypeUtils::isTankObject(obj_type) &&
                 GameObjectTypeUtils::tankTypeToPlayerIndex(obj_type) == this->player_idx)
            allies_locations.insert(location);
    }

    void SatelliteAnalyticsView::removeObjectInternal(const BoardCell &location, GameObjectType obj_type)
    {
        // remove from map
        auto &objects = this->analytical_view[location]; // reference so delete will effect correcty
        for (int i = objects.size() - 1; i >= 0; --i)    // iterate backwards to avoid index out of bounds or skipping
        {
            if (objects[i].first == obj_type)
                objects.erase(objects.begin() + i);
        }

        // remove from type sets
        if (obj_type == GameObjectType::SHELL)
            shells_locations.erase(location);

        else if (obj_type == GameObjectType::MINE)
            mines_locations.erase(location);

        else if (GameObjectTypeUtils::isTankObject(obj_type) &&
                 GameObjectTypeUtils::tankTypeToPlayerIndex(obj_type) != this->player_idx)
            enemies_locations.erase(location);

        else if (GameObjectTypeUtils::isTankObject(obj_type) &&
                 GameObjectTypeUtils::tankTypeToPlayerIndex(obj_type) == this->player_idx)
            allies_locations.erase(location);
    }

    void SatelliteAnalyticsView::advanceShellsOnce()
    {
        /*
            advance all shells 1 cell in their assumed direction.
            Note: shells without known direction are assumed to be going down 

            collisions of shells with other objects are handled in the following way:
                if shell collides with shell, both shells are removed
                if shell collides with wall, both wall and shell are removed (wall is assumed to be 1 hp)
                if shell collides with tank, none are removed (assuming tank evaded shell)
        */

        std::set<BoardCell> old_shells_locations = this->getShellsLocations();
        AssumedDirection assumed_dir;
        std::vector<std::pair<BoardCell, AssumedDirection>> temp;
        for (BoardCell old_shell_location : old_shells_locations)
        {
            std::optional<AssumedDirection> assumed_dir_opt = this->getDirectionOfObjectAt(GameObjectType::SHELL, old_shell_location);
            if (!assumed_dir_opt) // shell not exist on old location - Should never happen
                continue;

            assumed_dir = *assumed_dir_opt;
            if (assumed_dir == AssumedDirection::UNKNOWN)
                assumed_dir = AssumedDirection::DOWN;

            Direction shell_dir = static_cast<Direction>(assumed_dir);
            BoardCell new_shell_location = this->createAdjustedBoardCell(old_shell_location + shell_dir);
            // remove from previous cell
            removeObjectInternal(old_shell_location, GameObjectType::SHELL);

            temp.push_back({new_shell_location, *assumed_dir_opt});
        }

        for (auto [shell_loc, ass_dir] : temp)
        {
            // add to new cell
            addObjectInternal(shell_loc, GameObjectType::SHELL, ass_dir);

            // update flag for tracked location
            if (shell_loc == this->monitored_location)
            {
                this->monitored_cell_flag = true;
            }
        }

        // handle end of step collisions with walls.
        for (BoardCell new_shell_location : this->getShellsLocations())
        {
            auto types_on_cell = this->getObjectsTypesOnCell(new_shell_location);
            if (types_on_cell.find(GameObjectType::WALL) != types_on_cell.end())
            {
                this->removeObjectInternal(new_shell_location, GameObjectType::SHELL);
                this->removeObjectInternal(new_shell_location, GameObjectType::WALL);
            }
        }
    }

    // === Init Helpers (Private) === //
    void SatelliteAnalyticsView ::clearView()
    {
        shells_locations.clear();
        enemies_locations.clear();
        allies_locations.clear();
        mines_locations.clear();
        analytical_view.clear();
    }

    void SatelliteAnalyticsView::initAnalyticalView(const SatelliteView &sat_view)
    {
        this->clearView();
        for (size_t y = 0; y < getHeight(); ++y)
        {
            for (size_t x = 0; x < getWidth(); ++x)
            {
                char obj_char = sat_view.getObjectAt(x, y);
                if (!GameObjectTypeUtils::isValidObjectChar(obj_char)) // empty/out of bounds/caller tank
                    continue;

                GameObjectType obj_type = static_cast<GameObjectType>(obj_char);
                addObjectInternal(BoardCell(x, y), obj_type, AssumedDirection::UNKNOWN);
            }
        }
    }

    BoardCell SatelliteAnalyticsView::shellExpectedLocation(const BoardCell &old_location, AssumedDirection assumed_dir, size_t steps_gap) const
    {
        // assuming that the satellite view is taken at the beginning of the step - before shell movement happened

        return this->createAdjustedBoardCell(old_location + (static_cast<Direction>(assumed_dir) * static_cast<int>(steps_gap * ConfigReader::getConfig().getShellsSpeed())));
    }

    bool SatelliteAnalyticsView::isDirectionMatchShell(const BoardCell &old_location, const BoardCell &new_location, AssumedDirection assumed_dir, size_t steps_gap) const
    {

        // check if new location contains a shell
        std::unordered_set<GameObjectType> objects_types_on_cell = this->getObjectsTypesOnCell(new_location);
        if (objects_types_on_cell.find(GameObjectType::SHELL) == objects_types_on_cell.end())
            return false;

        // check if path to new location does not have walls
        BoardCell cell_to_check = old_location;
        for (size_t i = 0; i < steps_gap; ++i)
        {
            cell_to_check = cell_to_check + static_cast<Direction>(assumed_dir);
            std::unordered_set<GameObjectType> objects_types_on_cell = this->getObjectsTypesOnCell(cell_to_check);
            if (objects_types_on_cell.find(GameObjectType::WALL) != objects_types_on_cell.end())
                return false;
        }
        return true;
    }

    AssumedDirection SatelliteAnalyticsView::findMatchingShellDirection(const BoardCell &old_location, size_t steps_gap) const
    {
        AssumedDirection dir;
        for (size_t i = 0; i < 8; ++i)
        {
            dir = static_cast<AssumedDirection>(i);
            BoardCell new_location = shellExpectedLocation(old_location, dir, steps_gap);
            if (isDirectionMatchShell(old_location, new_location, dir, steps_gap))
                return dir;
        }
        return AssumedDirection::UNKNOWN;
    }

    // === Public Functions === //
    // === Getters === //
    size_t SatelliteAnalyticsView::getMaxStepGap() const
    {
        return this->max_steps_gap;
    }

    std::set<BoardCell> SatelliteAnalyticsView::getEnemyTanksLocations() const
    {
        return this->enemies_locations;
    }

    std::set<BoardCell> SatelliteAnalyticsView::getAlliesTanksLocations() const
    {
        return this->allies_locations;
    }

    std::set<BoardCell> SatelliteAnalyticsView::getShellsLocations() const
    {
        return this->shells_locations;
    }

    std::set<BoardCell> SatelliteAnalyticsView::getMinesLocations() const
    {
        return this->mines_locations;
    }

    size_t SatelliteAnalyticsView::getEnemyTanksNum() const
    {
        return this->enemies_locations.size();
    }

    std::vector<std::pair<char, AssumedDirection>> SatelliteAnalyticsView::getObjectsAt(const BoardCell &c) const
    {
        BoardCell adjusted_cell = this->createAdjustedBoardCell(c);
        return getObjectsAtInternal(adjusted_cell);
    }

    // Const version - return a copy wrapped in std::optional
    std::optional<AssumedDirection> SatelliteAnalyticsView::getDirectionOfObjectAt(GameObjectType t, const BoardCell &c) const
    {
        auto object_opt = getObjectAt(t, c);
        if (!object_opt)
            return std::nullopt;

        return object_opt->second; // returns a copy of AssumedDirection
    }

    std::optional<std::pair<GameObjectType, AssumedDirection>> SatelliteAnalyticsView::getObjectAt(GameObjectType requested_type, const BoardCell &location) const
    {
        BoardCell adjusted_cell = createAdjustedBoardCell(location);
        auto it = analytical_view.find(adjusted_cell);
        if (it == analytical_view.end())
            return std::nullopt;

        auto &objects = it->second;

        for (auto &object_pair : objects)
        {
            if (object_pair.first == requested_type)
                return object_pair; // returns a copy
        }

        return std::nullopt;
    }

    int SatelliteAnalyticsView::getAlliesTanksNum() const
    {
        return this->allies_locations.size() + 1;
    }

    // === Override === //
    std::unordered_set<GameObjectType> SatelliteAnalyticsView::getObjectsTypesOnCell(const BoardCell &c) const
    {

        std::vector<std::pair<char, AssumedDirection>> objects_on_cell = getObjectsAt(c);
        std::unordered_set<GameObjectType> object_types_on_cell;

        for (auto obj_pair : objects_on_cell)
        {
            object_types_on_cell.insert(static_cast<GameObjectType>(obj_pair.first));
        }

        return object_types_on_cell;
    }

    std::vector<BoardCell> SatelliteAnalyticsView::getOccupiedCells() const
    {
        std::vector<BoardCell> occupied_cells;
        for (auto iter : this->analytical_view)
        {
            occupied_cells.push_back(iter.first);
        }

        return occupied_cells;
    }

    // === Movement validation === //
    bool SatelliteAnalyticsView::isWallOnCell(const BoardCell &cell) const
    {
        std::unordered_set<GameObjectType> objects_on_cell = this->getObjectsTypesOnCell(cell);

        return objects_on_cell.find(GameObjectType::WALL) != objects_on_cell.end();
    }

    // === Update View Functions === //
    void SatelliteAnalyticsView::updateAnalyticalView(const SatelliteView &sat_view, size_t current_step)
    {
        size_t sat_view_creation_step = current_step; // created as of last step
        size_t steps_gap = sat_view_creation_step - last_updated_step;
        last_updated_step = current_step;

        if (steps_gap > max_steps_gap)
        {
            this->initAnalyticalView(sat_view);
            return;
        }

        // update analytical view with directions
        // add all objects to analytical view, add direction to relevant objects (moved exactly (speed * steps) squares, no obstacles in the way)
        std::set<BoardCell> old_shells_locations = this->shells_locations;
        AnalyticalViewMap old_map = this->analytical_view;
        this->initAnalyticalView(sat_view);

        // iterate on all shells on old map.
        // if shell had direction, check if there is a shell in expected location and pass on the assumed direction
        // if shell did not have a direction, check all possible destinations.
        for (BoardCell cell : old_shells_locations)
        {
            auto iter = old_map.find(cell);
            if (iter == old_map.end())
                continue;

            for (auto &[obj_type, prev_dir] : iter->second)
            {

                if (obj_type != GameObjectType::SHELL)
                    continue;

                AssumedDirection assumed_dir = prev_dir;
                if (assumed_dir == AssumedDirection::UNKNOWN)
                {
                    assumed_dir = findMatchingShellDirection(cell, steps_gap);
                    if (assumed_dir == AssumedDirection::UNKNOWN)
                        continue;
                    BoardCell new_cell = shellExpectedLocation(cell, assumed_dir, steps_gap);
                    auto &objects_at_new_cell = analytical_view[new_cell];
                    for (auto &obj_pair : objects_at_new_cell)
                    {
                        if (obj_pair.first == GameObjectType::SHELL)
                        {
                            obj_pair.second = assumed_dir;
                            break;
                        }
                    }
                }
            }
        }
    }

    void SatelliteAnalyticsView::applyApproxBoardChanges()
    {
        this->monitored_cell_flag = false; // reset tracking flag
        advanceShells();
        this->monitored_location = BoardCell(-1, -1); // discard location, no longer tracking
    }

    void SatelliteAnalyticsView::updateShellsDirectionsFromView(const SatelliteAnalyticsView &other)
    {
        for (auto shell_location : getShellsLocations())
        {
            for (auto &[obj_type, assumed_dir] : analytical_view[shell_location])
            {
                if (obj_type != GameObjectType::SHELL || assumed_dir != AssumedDirection::UNKNOWN)
                    continue;

                auto other_opt_dir = other.getDirectionOfObjectAt(GameObjectType::SHELL, shell_location);
                if (!other_opt_dir)
                    continue;

                assumed_dir = *other_opt_dir;
            }
        }
    }

    // === Move Objects on View === //
    void SatelliteAnalyticsView::advanceShells()
    {
        for (size_t i = 0; i < ConfigReader::getConfig().getShellsSpeed(); i++)
        {
            advanceShellsOnce();
        }
    }

    void SatelliteAnalyticsView::addShell(const BoardCell &location, Direction dir)
    {
        BoardCell adjusted_location = this->createAdjustedBoardCell(location);
        addObjectInternal(adjusted_location, GameObjectType::SHELL, static_cast<AssumedDirection>(dir));
    }

    // === Setters === //
    void SatelliteAnalyticsView::setMonitoredCell(BoardCell cell)
    {
        this->monitored_location = cell;
    }

    // ===  === //
    bool SatelliteAnalyticsView::isMonidoredCellHit()
    {
        return monitored_cell_flag;
    }

    int SatelliteAnalyticsView::getTankDifference() const
    {
        return getAlliesTanksNum() - getEnemyTanksNum();
    }
}