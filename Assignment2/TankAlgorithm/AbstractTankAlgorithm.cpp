#include "AbstractTankAlgorithm.h"
#include <iostream>
// === Constructor === //
AbstractTankAlgorithm::AbstractTankAlgorithm(size_t tank_idx, size_t player_idx)
    : tank_idx(tank_idx), player_idx(player_idx),
      sat_view(0, 0, 0, player_idx)

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

void AbstractTankAlgorithm::setMaxSteps(size_t max_steps)
{
    this->max_steps = max_steps;
}

// === Getters === //
size_t AbstractTankAlgorithm::getRemainingShells() const { return num_of_shells; }
BoardCell AbstractTankAlgorithm::getCurrentLocation() const { return assumed_location; }
Direction AbstractTankAlgorithm::getTankDirection() const { return direction; }
size_t AbstractTankAlgorithm::getCurrentStep() const { return current_step; }
size_t AbstractTankAlgorithm::getTankIdx() const { return tank_idx; }
GameObjectType AbstractTankAlgorithm::getTankType() const { return GameObjectTypeUtils::playerIndexToTankType(this->player_idx); }

// === Cooldown / Wait Management ===
bool AbstractTankAlgorithm::hasShells() const
{
    return this->getRemainingShells() > 0;
}
bool AbstractTankAlgorithm::canTankShoot() const
{
    return !cooldowns.isPendingBackwards() && cooldowns.isDoneShootCooldown() && hasShells();
}

void AbstractTankAlgorithm::executeShoot()
{
    if (canTankShoot())
    {
        BoardCell shell_location = this->assumed_location + this->direction;
        this->sat_view.addShell(shell_location, this->direction);

        this->cooldowns.beginShoot();
        this->setRemainingShells(this->getRemainingShells() - 1);
    }
}

bool AbstractTankAlgorithm::canMoveToCell(const BoardCell &cell) const
{
    return !sat_view.isWallOnCell(cell);
}
void AbstractTankAlgorithm::handlePendingBackwards()
{
    // this function moves the tank backwards on the end of cooldown.
    // if movement is not possible, cancel the backwards wait

    if (cooldowns.isDoneBackwardWait())
    {
        // if can move backwards - move
        BoardCell backwards_cell = GameBoardUtils::createAdjustedBoardCell(this->getCurrentLocation() - this->getTankDirection(), this->sat_view.getWidth(), this->sat_view.getHeight());
        if (canMoveToCell(backwards_cell))
        {
            this->setCurrentLocation(backwards_cell);
        }
        else
        {
            cooldowns.cancelBackwardWait();
        }
    }
}

// === Steps Logic === //
void AbstractTankAlgorithm::advanceStep()
{
    current_step++;
    cooldowns.tickCooldowns();
    handlePendingBackwards();

    this->sat_view.approxBoardChanges();
}

// === Action Planning ===
ActionRequest AbstractTankAlgorithm::getTankEvasionAction(const SatelliteAnalyticsView &sat_view, Direction chaser_direction) const
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

ActionRequest AbstractTankAlgorithm::getTankAggressiveAction(const SatelliteAnalyticsView &sat_view) const
{
    // try to chase the enemy tank or shoot at it.

    // dijkstra to closest tank
    // if can shoot it - shoot
    // if can't shoot it - chase

    BoardCell start = this->assumed_location;
    BoardCell target = this->approxClosestEnemyTankLocation(sat_view);

    // try to shoot enemy
    if (auto shoot_action_opt = this->evaluateShootingOpportunity(target, sat_view.getWidth(), sat_view.getHeight()))
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

ActionRequest AbstractTankAlgorithm::advanceTankToTarget(const SatelliteAnalyticsView &sat_view, BoardCell target) const
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
    return ActionRequest::DoNothing; // in correct direction
}

// === Target Estimation ===
BoardCell AbstractTankAlgorithm::approxClosestEnemyTankLocation(const SatelliteAnalyticsView &sat_view) const
{
    /*  this function gives an approximation of the closest enemy tank location.
        to be clear, the approximation is regarding the closest tank identity. the location is not an approximation.

        using direct distance to find distance in a fast way
    */
    BoardCell closest = this->assumed_location;
    int closest_dist = -1;
    for (BoardCell enemy_loc : sat_view.getEnemyTanksLocations())
    {
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
void AbstractTankAlgorithm::Dijkstra(const SatelliteAnalyticsView &sat_view, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents) const
{
    std::priority_queue<
        std::pair<int, std::pair<BoardCell, BoardCell>>,
        std::vector<std::pair<int, std::pair<BoardCell, BoardCell>>>,
        std::greater<std::pair<int, std::pair<BoardCell, BoardCell>>>>
        q;
    q.push({0, {start, start}});
    std::set<BoardCell> visited;
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
            if (neighbor == target || GameCollisionHandler::canObjectSafelyStepOn(sat_view, tank_type, neighbor))
            {

                q.push({dist + 1, {neighbor, c}});
            }
        }
    }
}

// === Target Evaluation ===
std::optional<ActionRequest> AbstractTankAlgorithm::evaluateShootingOpportunity(BoardCell target, size_t width, size_t height) const
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

std::optional<ActionRequest> AbstractTankAlgorithm::escapeShells(const SatelliteAnalyticsView &sat_view) const
{
    size_t step_dist = 5;
    step_dist = std::min(std::min(sat_view.getHeight(), sat_view.getWidth()), step_dist); // for small fields.
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

            auto objects = sat_view.getObjectsAt(cell);
            for (auto [object_type_char, object_assumed_direction] : objects)
            {

                if (object_type_char != static_cast<char>(GameObjectType::SHELL))
                    continue;

                if (object_assumed_direction == AssumedDirection::UNKNOWN)
                    continue;

                Direction obj_direction = static_cast<Direction>(object_assumed_direction);

                if (GameBoardUtils::isDirectionMatch(cell, this->assumed_location, obj_direction, sat_view.getWidth(), sat_view.getHeight()))
                {
                    return getTankEvasionAction(sat_view, obj_direction);
                }
            }
        }
    }

    return std::nullopt;
}

std::optional<BoardCell> AbstractTankAlgorithm::getEscapingRoute(SatelliteAnalyticsView sat_view, Direction enemy_dir) const
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
    this->advanceStep();
    ActionRequest request = this->getActionLogic();
    this->adjustSelfToAction(request);

    // TODO: this is for debugging the sat_view, delete later
    if (request == ActionRequest::Shoot)
    {
        this->sat_view.print();
    }

    return request;
}

void AbstractTankAlgorithm::updateBattleInfo(BattleInfo &info)
{
    auto current_info = dynamic_cast<BattleInfoAgent &>(info);

    // if first time getting battle info - must init some of the details from it.
    if (this->getCurrentStep() == 1)
    {
        // set game settings and save them
        this->setRemainingShells(current_info.getInitialNumShells());
        this->setMaxSteps(current_info.getMaxSteps());

        // height, width of map are accesible through the sat_view
    }

    // correction to location
    this->setCurrentLocation(current_info.getTankLocation());
    this->step_to_get_info = current_info.getStepToGetInfo(this->getCurrentStep());

    // get sat_view from BattleInfo
    current_info.updateViewForStep(this->getCurrentStep());
    // TODO: here there should be some function that uses the algorithm analytics map to update the player one (using the battle info)
    this->sat_view = current_info.getAnalyticsView();

    // update BattleInfo with algorithm data (tank_to_player_details)
    current_info.setCurrentStep(this->getCurrentStep());
    current_info.setTankDirection(this->direction);
    current_info.setRemainingShells(this->getRemainingShells());
}

//=== Helper Functions ===
void AbstractTankAlgorithm::adjustSelfToAction(ActionRequest action)
{
    // === Adjust self to action ===
    switch (action)
    {
    case ActionRequest::GetBattleInfo:
        break;

    case ActionRequest::MoveForward:

        if (cooldowns.isPendingBackwards())
        {
            cooldowns.cancelBackwardWait();
        }
        else
        {

            BoardCell forward_cell = GameBoardUtils::getNextCellInDirection(
                this->assumed_location,
                this->direction,
                sat_view.getWidth(),
                sat_view.getHeight());

            if (canMoveToCell(forward_cell))
            {
                this->setCurrentLocation(forward_cell);
            }
        }
        break;

    case ActionRequest::MoveBackward:
        if (cooldowns.isPendingBackwards())
        {
            break;
        }
        else if (cooldowns.canImmediateBackwards())
        {
            BoardCell backward_cell = GameBoardUtils::getNextCellInDirection(
                this->assumed_location,
                DirectionUtils::getOppositeDirection(this->direction),
                sat_view.getWidth(),
                sat_view.getHeight());
            if (canMoveToCell(backward_cell)){
                this->assumed_location = backward_cell;
                cooldowns.extendBackwardsStreak();
            }
            else{
                cooldowns.cancelBackwardWait();
            }
        }
        else
        {
            cooldowns.startBackwardWait();
        }
        break;

    // TODO: extract this to a function
    case ActionRequest::RotateLeft45:
    case ActionRequest::RotateLeft90:
    case ActionRequest::RotateRight45:
    case ActionRequest::RotateRight90:
        if (!cooldowns.isPendingBackwards())
        {
            int steps = (action == ActionRequest::RotateLeft45 || action == ActionRequest::RotateRight45) ? 1 : 2;
            bool isLeft = (action == ActionRequest::RotateLeft45 || action == ActionRequest::RotateLeft90);
            this->direction = isLeft
                                  ? DirectionUtils::rotateLeft(this->direction, steps)
                                  : DirectionUtils::rotateRight(this->direction, steps);
        }
        break;

    case ActionRequest::Shoot:
        this->executeShoot();
        break;

    case ActionRequest::DoNothing:
    default:
        break;
    }
}
