#include "AbstractTankAlgorithm.h"

#include <iostream>
// === Constructor === //
AbstractTankAlgorithm::AbstractTankAlgorithm(size_t tank_idx, size_t player_idx)
    : tank_idx(tank_idx),
      player_idx(player_idx),
      sat_view(0, 0, 0,player_idx)
{
    // TODO: should later calculate based on config.h
    direction = player_idx % 2 == 1 ? Direction::LEFT : Direction::RIGHT;
}

// === Setters === //
void AbstractTankAlgorithm::setCurrentLocation(const BoardCell &loc)
{
    this->assumed_location = loc;
}

void AbstractTankAlgorithm::setRemainingShells(size_t shells)
{
    this->num_of_shells = shells;
}

void AbstractTankAlgorithm::setCurrentStep(size_t step)
{
    this->current_step = step;
}

void AbstractTankAlgorithm::setTankDirection(Direction dir)
{
    this->direction = dir;
}

void AbstractTankAlgorithm::setTankIndex(size_t idx)
{
    this->tank_idx = idx;
}

// === Getters === //
size_t AbstractTankAlgorithm::getRemainingShells() const { return num_of_shells; }
BoardCell AbstractTankAlgorithm::getCurrentLocation() const { return assumed_location; }
Direction AbstractTankAlgorithm::getTankDirection() const { return direction; }
size_t AbstractTankAlgorithm::getCurrentStep() const { return current_step; }
size_t AbstractTankAlgorithm::getShootCooldown() const { return shoot_cooldown; }
size_t AbstractTankAlgorithm::getTankIdx() const { return tank_idx; }
GameObjectType AbstractTankAlgorithm::getTankType() const { return GameObjectTypeUtils::playerIndexToTankType(this->player_idx); }

// === Shoot Cooldown Management === //
bool AbstractTankAlgorithm::canTankShoot() const
{
    return (this->shoot_cooldown == 0);
}

void AbstractTankAlgorithm::execute_shoot()
{
    shoot_cooldown = 4;
}

// === Steps Logic === //
void AbstractTankAlgorithm::advance_step()
{
    current_step++;
    shoot_cooldown--;
}

// === Action Planning ===
ActionRequest AbstractTankAlgorithm::getTankEvasionAction(const SatelliteAnalyitcsView &sat_view, BoardCell chaser_location, Direction chaser_direction) const
{
    if (chaser_direction == this->getTankDirection())
    {
        if (auto opt_escape_cell = getEscapingRoute(sat_view, chaser_direction))
        {
            return adjustDirection(opt_escape_cell.value(), sat_view.getWidth(), sat_view.getHeight());
        }
        return ActionRequest::Shoot;
    }
    else if (chaser_direction == DirectionUtils::getOppositeDirection(this->getTankDirection()))
    {
        if (auto opt_escape_cell = getEscapingRoute(sat_view, chaser_direction))
        {
            return adjustDirection(opt_escape_cell.value(), sat_view.getWidth(), sat_view.getHeight());
        }
        return ActionRequest::Shoot;
    }
    else
    {
        return ActionRequest::MoveForward;
    }
}

ActionRequest AbstractTankAlgorithm::getTankAggressiveAction(const SatelliteAnalyitcsView &sat_view) const
{
    // try to chase the enemy tank or shoot at it.

    // dijkstra to closest tank
    // if can shoot it - shoot
    // if can't shoot it - chase

    BoardCell start = this->assumed_location;
    BoardCell target = this->approxClosestEnemyTankLocation(sat_view);

    // try to shoot enemy
    if (auto shoot_action_opt = this->attemptShoot(target, sat_view.getWidth(), sat_view.getHeight()))
    {
        return shoot_action_opt.value();
    }

    // if will be able to shoot in the future, wait
    if (TankAlgorithmUtils::inShootRange(start, target, sat_view.getWidth(), sat_view.getHeight()) &&
        GameBoardUtils::isStraightLine(start, target, sat_view.getWidth(), sat_view.getHeight()) && this->getRemainingShells() > 0)
    {
        return ActionRequest::DoNothing;
    }

    return advanceTankToTarget(sat_view, target);
}

ActionRequest AbstractTankAlgorithm::advanceTankToTarget(const SatelliteAnalyitcsView &sat_view, BoardCell target) const
{
    BoardCell start = this->assumed_location;
    map<BoardCell, int> distances;
    map<BoardCell, BoardCell> parents;
    this->Dijkstra(sat_view, this->getTankType(), this->assumed_location, target, distances, parents);

    // TODO: if cant reach target shoot and hope for good. maybe should do something smarter?
    if (distances.find(target) == distances.end())
    {
        return ActionRequest::Shoot;
    }

    // getting the next cell in the shortest path
    BoardCell next_cell_in_path = target;
    while (parents[next_cell_in_path] != start)
    {
        next_cell_in_path = parents[next_cell_in_path];
    }
    Logger::runtime().logLine("attempting to advance to: " + std::to_string(next_cell_in_path.getX()) + "," + std::to_string(next_cell_in_path.getY()));
    // decisions - try to advance to next cell
    if (next_cell_in_path == GameBoardUtils::getNextCellInDirection(start, this->getTankDirection(), sat_view.getWidth(), sat_view.getHeight()))
    {
        return ActionRequest::MoveForward;
    }
    if (next_cell_in_path == GameBoardUtils::getNextCellInDirection(start, DirectionUtils::getOppositeDirection(this->getTankDirection()), sat_view.getWidth(), sat_view.getHeight()))
    {
        return ActionRequest::MoveBackward;
    }
    return adjustDirection(next_cell_in_path, sat_view.getWidth(), sat_view.getHeight());
}

ActionRequest AbstractTankAlgorithm::adjustDirection(BoardCell to, size_t width, size_t height) const
{
    // assumption: from, to are adjacent cells
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 1), width, height))
    {
        return ActionRequest::RotateRight45;
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 2), width, height))
    {
        return ActionRequest::RotateRight90;
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 3), width, height))
    {
        return ActionRequest::RotateRight90; // will need additional 45 deg turn
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 1), width, height))
    {
        return ActionRequest::RotateLeft45;
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 2), width, height))
    {
        return ActionRequest::RotateLeft90;
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 3), width, height))
    {
        return ActionRequest::RotateLeft90; // will need additional 45 deg turn
    }
    if (to == GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 4), width, height))
    {
        return ActionRequest::RotateLeft90; // will need additional 90 deg turn
    }
    Logger::runtime().logLine("can't adjust direction to ..." + std::to_string(to.getX()) + "," + std::to_string(to.getY()));
    return ActionRequest::DoNothing; // in correct direction
}

// === Target Estimation ===
BoardCell AbstractTankAlgorithm::approxClosestEnemyTankLocation(const SatelliteAnalyitcsView &sat_view) const
{
    /*  this function gives an approximation of the closest enemy tank location.
        to be clear, the approximation is regarding the closest tank identity. the location is not an approximation.

        using direct distance to find distance in a fast way
    */
    BoardCell closest = this->assumed_location;
    int closest_dist = -1;
    for (BoardCell enemy_loc : sat_view.getEnemyTanksLocations())
    {
        Logger::runtime().logLine("checking enemy location " + std::to_string(enemy_loc.getX())+"," +std::to_string(enemy_loc.getY()));
        int curr_dist = GameBoardUtils::distance(closest, enemy_loc, sat_view.getWidth(), sat_view.getHeight());
        if (closest_dist == -1 || closest_dist > curr_dist)
        {
            closest_dist = curr_dist;
            closest = enemy_loc;
        }
    }
    return closest;
}

// === Pathfinding ===
void AbstractTankAlgorithm::Dijkstra(const SatelliteAnalyitcsView &sat_view, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents) const
{
    std::priority_queue<
        std::pair<int, std::pair<BoardCell, BoardCell>>,
        std::vector<std::pair<int, std::pair<BoardCell, BoardCell>>>,
        std::greater<std::pair<int, std::pair<BoardCell, BoardCell>>>>
        q;
    q.push({0, {start, start}});
    std::set<BoardCell> visited;
    Logger::runtime().logLine("running dijkstra on board of " + std::to_string(sat_view.getWidth()) + "," + std::to_string(sat_view.getHeight()));
    Logger::runtime().logLine("dijkstra to: (" + std::to_string(target.getX()) + "," +std::to_string(target.getY()) + ")");
    while (!q.empty())
    {
        // access and pop first item in heap
        auto [dist, current_and_parent_pair] = q.top();
        auto [c, parent] = current_and_parent_pair;
        q.pop();

        // skip visited since inserting cells multiple times
        if (visited.find(c) != visited.end())
        {
            continue;
        }

        // mark visted and save distance, parent data
        visited.insert(c);
        parents[c] = parent;
        distances[c] = dist;

        // early termination when reaching target
        if (c == target)
            break;

        // adding neighbors to heap - only if tank can safely step there
        for (BoardCell neighbor : GameBoardUtils::getAdjacentCells(c, sat_view.getWidth(), sat_view.getHeight()))
        {
            // TODO: this is a test - delete it
            if (sat_view.getObjectAt(neighbor.getX(), neighbor.getY()).first == static_cast<char>(GameObjectType::MINE)){
                Logger::runtime().logLine("FOUND MINE on (" + std::to_string(neighbor.getX()) + "," + std::to_string(neighbor.getY()) + ")");
            }
            if (neighbor == target || GameCollisionHandler::canObjectSafelyStepOn(sat_view, tank_type, neighbor))
            {
                // TODO: this is a test - delete it
                Logger::runtime().logLine("adding neighbour (" + std::to_string(neighbor.getX()) + "," + std::to_string(neighbor.getY()) + ")");

                q.push({dist + 1, {neighbor, c}});
            }
        }
    }
}

// === Target Evaluation ===
// TODO: change function name - it is not actually attempting the shot but checking if should shoot from here or adjust
// evaluateShootingOpportunity
std::optional<ActionRequest> AbstractTankAlgorithm::attemptShoot(BoardCell target, size_t width, size_t height) const
{
    BoardCell start = this->assumed_location;
    if (TankAlgorithmUtils::inShootRange(start, target, width, height) && this->canTankShoot())
    {
        if (GameBoardUtils::isDirectionMatch(start, target, this->getTankDirection(), width, height))
            return ActionRequest::Shoot;

        else if (GameBoardUtils::isStraightLine(start, target, width, height))
        {
            BoardCell adjust_to = GameBoardUtils::getNextCellInStraightLine(start, target, width, height);
            return adjustDirection(adjust_to, width, height);
        }
        else
        { // Cell is not in direct range to target. can do nothing here.
        }
    }
    return std::nullopt;
}

std::optional<ActionRequest> AbstractTankAlgorithm::escapeShells(const SatelliteAnalyitcsView &sat_view) const
{
    size_t step_dist = 3;
    step_dist = std::min(std::min(sat_view.getHeight(), sat_view.getWidth()), step_dist); // for small fields.
    int shell_speed = 2;                                                                  // TODO: add configured param
    int proximitiy = step_dist * shell_speed;

    int left = this->assumed_location.getX() - proximitiy;
    int right = this->assumed_location.getX() + proximitiy;
    int up = this->assumed_location.getY() - proximitiy;
    int down = this->assumed_location.getY() + proximitiy;

    for (int x = left; x <= right; ++x)
    {
        for (int y = up; y <= down; ++y)
        {
            BoardCell cell = GameBoardUtils::createAdjustedBoardCell(BoardCell(x, y), sat_view.getWidth(), sat_view.getWidth());

            auto [object_type_char, object_assumed_direction] = sat_view.getObjectAt(cell.getX(), cell.getY());
            if (object_type_char != static_cast<char>(GameObjectType::SHELL))
                continue;

            if (object_assumed_direction == AssumedDirection::UNKNOWN)
                continue;

            Direction obj_direction = static_cast<Direction>(object_assumed_direction);

            if (GameBoardUtils::isDirectionMatch(cell, this->assumed_location, obj_direction, sat_view.getWidth(), sat_view.getHeight()))
            {
                return getTankEvasionAction(sat_view, cell, obj_direction);
            }
        }
    }

    return std::nullopt;
}

std::optional<BoardCell> AbstractTankAlgorithm::getEscapingRoute(SatelliteAnalyitcsView sat_view, Direction enemy_dir) const
{
    std::vector<BoardCell> neighbors = GameBoardUtils::getAdjacentCells(this->assumed_location, sat_view.getHeight(), sat_view.getWidth());
    for (BoardCell neighbor : neighbors)
    {
        // check if stepped out from enemy direction
        if (GameBoardUtils::isDirectionMatch(this->assumed_location, neighbor, enemy_dir, sat_view.getWidth(), sat_view.getHeight()) ||
            GameBoardUtils::isDirectionMatch(this->assumed_location, neighbor, DirectionUtils::getOppositeDirection(enemy_dir), sat_view.getWidth(), sat_view.getHeight()))
            continue;

        if (GameCollisionHandler::canObjectSafelyStepOn(sat_view, this->getTankType(), neighbor))
        {
            return neighbor;
        }
    }

    return std::nullopt;
}

//=== Interface ===
ActionRequest AbstractTankAlgorithm::getAction()
{
    ActionRequest request = this->getActionLogic();
    Logger::runtime().logLine("assumed direction of player" +std::to_string(player_idx) +"before step is " + std::to_string(static_cast<int>(this->direction)));
    this->adjustSelfToAction(request);
    Logger::runtime().logLine("assumed direction of player" +std::to_string(player_idx) + "after step is " + std::to_string(static_cast<int>(this->direction)));
    return request;
}

//=== Helper Functions ===
void AbstractTankAlgorithm::adjustSelfToAction(ActionRequest action)
{
    switch (action)
    {
    case ActionRequest::MoveForward:
        this->assumed_location = GameBoardUtils::getNextCellInDirection(this->assumed_location, this->getTankDirection(), sat_view.getWidth(), sat_view.getHeight());
        break;

    case ActionRequest::MoveBackward:
        // TODO: fix the backwards logic with the wait
        //  this->assumed_location = GameBoardUtils::getNextCellInDirection(this->assumed_location, DirectionUtils::getOppositeDirection(this->getTankDirection()), sat_view.getWidth(), sat_view.getHeight());
        break;

    case ActionRequest::RotateLeft45:
        this->direction = DirectionUtils::rotateLeft(this->getTankDirection(), 1);
        break;

    case ActionRequest::RotateLeft90:
        this->direction = DirectionUtils::rotateLeft(this->getTankDirection(), 2);
        break;

    case ActionRequest::RotateRight45:
        this->direction = DirectionUtils::rotateRight(this->getTankDirection(), 1);
        break;

    case ActionRequest::RotateRight90:
        this->direction = DirectionUtils::rotateRight(this->getTankDirection(), 2);
        break;

    case ActionRequest::Shoot:
        // TODO:fix the execute shoot function to only change the coolodwn when can actually shoot
        this->execute_shoot();
        break;

    case ActionRequest::DoNothing:
    default:
        break;
    }
}
