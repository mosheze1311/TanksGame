#include "../config.h"
#include "SatelliteAnalyticsView.h"
#include "../GameBoardUtils/GameBoardUtils.h"

#include <iostream>

/*
    TODO: this class contains logic errors.
    need to fix the following (and maybe more): 
        1. mix TankAlgorithm's object with Player's object on the GetBattleInfo situation
        2. moving shells on board should die when colliding with other objects
        3. allowing multiple objects on the same cell
        4. I added a printing function for debugging - it should be deleted when done
        5. should use ajusted boardcells

    should test this class carefully
*/

// === Constructor === //
SatelliteAnalyitcsView::SatelliteAnalyitcsView(size_t height, size_t width, size_t max_steps_gap, size_t player_idx)
    : width(width), height(height), max_steps_gap(max_steps_gap), player_idx(player_idx) {};

//=== Destructor ===//
SatelliteAnalyitcsView ::~SatelliteAnalyitcsView() {}; // TODO: implement

// === Init Helpers (Private) === //
void SatelliteAnalyitcsView ::clearView()
{
    shells_locations.clear();
    enemies_locations.clear();
    mines_locations.clear();
    analytical_view.clear();
}
void SatelliteAnalyitcsView::initAnalyticalView(SatelliteView &sat_view)
{
    this->clearView();
    for (size_t x = 0; x < width; ++x)
    {
        for (size_t y = 0; y < height; ++y)
        {
            char obj_char = sat_view.getObjectAt(x, y);
            if (!GameObjectTypeUtils::isValidObjectChar(obj_char))
                continue;

            GameObjectType obj_type = static_cast<GameObjectType>(obj_char);
            analytical_view[BoardCell(x, y)] = {obj_type, AssumedDirection::UNKNOWN};

            if (obj_char == static_cast<char>(GameObjectType::SHELL))
                shells_locations.push_back(BoardCell(x, y));

            if (GameObjectTypeUtils::isTankObject(obj_type) && obj_type != GameObjectTypeUtils::playerIndexToTankType(this->player_idx))
            {
                enemies_locations.push_back(BoardCell(x, y));
            }

            if (obj_char == static_cast<char>(GameObjectType::MINE))
                mines_locations.push_back(BoardCell(x, y));
        }
    }
    enemy_estimate_tanks_num = enemies_locations.size();
}

BoardCell SatelliteAnalyitcsView::shellExpectedLocation(BoardCell &old_location, AssumedDirection assumed_dir, size_t steps_gap)
{
    // assuming that the satellite view is taken at the beginning of the step - before shell movement happened

    return old_location + (static_cast<Direction>(assumed_dir) * (steps_gap - 1));
}

bool SatelliteAnalyitcsView::isDirectionMatchShell(BoardCell &old_location, BoardCell &new_location, AssumedDirection assumed_dir, size_t steps_gap)
{
    // TODO: also check if not possible because would have collided with another shell

    // check if new location contains a shell
    if (this->analytical_view.find(new_location) == this->analytical_view.end() || this->analytical_view[new_location].first != GameObjectType::SHELL)
        return false;

    // check if path to new location does not have walls
    BoardCell cell_to_check = old_location;
    for (size_t i = 1; i <= steps_gap - 1; i++)
    {
        cell_to_check = cell_to_check + static_cast<Direction>(assumed_dir);
        auto object_on_cell_iter = this->analytical_view.find(cell_to_check);
        if (object_on_cell_iter == this->analytical_view.end())
            continue;

        // if wall, in path - direction does not match
        if (object_on_cell_iter->second.first == GameObjectType::WALL)
        {
            return false;
        }
    }

    return true;
}

AssumedDirection SatelliteAnalyitcsView::findMatchingShellDirection(BoardCell &old_location, size_t steps_gap)
{
    AssumedDirection dir;
    for (size_t i = 0; i < 8; i++)
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
size_t SatelliteAnalyitcsView::getWidth() const
{
    return this->width;
}

size_t SatelliteAnalyitcsView::getHeight() const
{
    return this->height;
};

size_t SatelliteAnalyitcsView::getMaxStepGap() const
{
    return this->max_steps_gap;
}

std::vector<BoardCell> SatelliteAnalyitcsView::getEnemyTanksLocations() const
{
    return this->enemies_locations;
}

std::vector<BoardCell> SatelliteAnalyitcsView::getShellsLocations() const
{
    return this->shells_locations;
}

std::vector<BoardCell> SatelliteAnalyitcsView::getMinesLocations() const
{
    return this->mines_locations;
}

size_t SatelliteAnalyitcsView::getEnemyTanksNum() const
{
    return this->enemy_estimate_tanks_num;
}

void SatelliteAnalyitcsView::addShell(BoardCell &location, Direction dir)
{
    location = GameBoardUtils::createAdjustedBoardCell(location, this->getWidth(), this->getHeight());
    this->analytical_view[location] = {GameObjectType::SHELL, static_cast<AssumedDirection>(dir)};
    this->shells_locations.push_back(location);
}

// TODO: improve function visibility
std::pair<char, AssumedDirection> SatelliteAnalyitcsView::getObjectAt(size_t x, size_t y) const
{
    if (x < 0 || x > this->width || y < 0 || y > this->height)
    {
        return {'&', AssumedDirection::UNKNOWN};
    }

    auto iter = this->analytical_view.find(BoardCell(x, y));
    if (iter == this->analytical_view.end())
    {
        return {' ', AssumedDirection::UNKNOWN};
    }

    return {static_cast<char>(iter->second.first), iter->second.second};
}

void SatelliteAnalyitcsView::updateAnalyticalView(SatelliteView &sat_view, size_t current_step)
{
    size_t steps_gap = current_step - last_updated_step;
    last_updated_step = current_step;
    if (steps_gap > max_steps_gap)
    {
        this->initAnalyticalView(sat_view);
        return;
    }

    // update analytical view with directions

    // add all objects to analytical view, add direction to relevant objects (moved exactly (speed * steps) squares, no obstacles in the way)
    std::vector<BoardCell> old_shells_locations = this->shells_locations;
    AnalyticalViewMap old_map = this->analytical_view;
    this->initAnalyticalView(sat_view);

    // iterate on all shells on old map.
    // if shell had direction, check if there is a shell in expected location and pass on the assumed direction
    // if shell did not have a direction, check all possible destinations.
    for (BoardCell &cell : old_shells_locations)
    {
        AssumedDirection assumed_dir = old_map[cell].second;

        if (assumed_dir == AssumedDirection::UNKNOWN)
        {
            assumed_dir = findMatchingShellDirection(cell, steps_gap);
            if (assumed_dir != AssumedDirection::UNKNOWN)
            {
                BoardCell new_cell = shellExpectedLocation(cell, assumed_dir, steps_gap);
                this->analytical_view[new_cell].second = assumed_dir;
            }
        }
        else
        {
            BoardCell new_cell = shellExpectedLocation(cell, assumed_dir, steps_gap);
            if (isDirectionMatchShell(cell, new_cell, assumed_dir, steps_gap))
                this->analytical_view[new_cell].second = assumed_dir;
        }
    }
}

void SatelliteAnalyitcsView::approxBoardChanges()
{
    advanceShells();
}

// === Move Objects on View === //
void SatelliteAnalyitcsView::advanceShells()
{
    for (int i = 0; i < shell_speed; i++)
    {
        advanceShellsOnce();
        // TODO: sheck for collisions and update map again.
    }
}

void SatelliteAnalyitcsView::advanceShellsOnce()
{
    std::vector<BoardCell> old_shells_locations = this->getShellsLocations();
    this->shells_locations.clear();
    for (size_t i = 0; i < old_shells_locations.size(); i++)
    {
        BoardCell old_shell_location = old_shells_locations[i];
        auto [obj_char, assumed_dir] = this->getObjectAt(old_shell_location.getX(), old_shell_location.getY());

        if (obj_char != static_cast<char>(GameObjectType::SHELL))
            continue; // should never happen

        AssumedDirection fixed_assumed_dir = assumed_dir;
        if (fixed_assumed_dir == AssumedDirection::UNKNOWN)
            fixed_assumed_dir = AssumedDirection::DOWN;

        Direction shell_dir = static_cast<Direction>(fixed_assumed_dir);
        std::cout << "sat view of " << this->player_idx << " thinks shell number " << i << " is going " << static_cast<int>(shell_dir) << std::endl;
        BoardCell new_shell_location = GameBoardUtils::createAdjustedBoardCell(old_shell_location + shell_dir, this->getWidth(), this->getHeight());

        // actual movement
        this->analytical_view.erase(old_shell_location);
        this->analytical_view[new_shell_location] = {GameObjectType::SHELL, assumed_dir},
        this->shells_locations.push_back(new_shell_location);
    }
}

// === DEBUGGING === //
#include <iostream> // TODO: delete include
#include <thread>   // TODO: delete include
#include <chrono>   // TODO: delete include
// TODO: this is temp function - delete or atleast move to SatelliteAnalyitcsView
void SatelliteAnalyitcsView::print()
{
    for (int y = 0; y < this->getHeight(); ++y)
    {
        for (int x = 0; x < this->getWidth(); ++x)
        {
            auto [object_type_char, object_assumed_direction] = this->getObjectAt(x, y);
            std::cout << object_type_char;

            if (object_type_char == static_cast<char>(GameObjectType::SHELL))
            {
                switch (object_assumed_direction)
                {
                case AssumedDirection::UNKNOWN:
                    std::cout << "??";
                    break;
                case AssumedDirection::UP:
                    std::cout << "^^";
                    break;
                case AssumedDirection::UPR:
                    std::cout << "^>";
                    break;
                case AssumedDirection::RIGHT:
                    std::cout << ">>";
                    break;
                case AssumedDirection::DOWNR:
                    std::cout << "v>";
                    break;
                case AssumedDirection::DOWN:
                    std::cout << "vv";
                    break;
                case AssumedDirection::DOWNL:
                    std::cout << "<v";
                    break;
                case AssumedDirection::LEFT:
                    std::cout << "<<";
                    break;
                case AssumedDirection::UPL:
                    std::cout << "<^";
                    break;
                default:
                    break;
                };
            }
            else
            {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}