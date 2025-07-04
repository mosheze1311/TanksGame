#include "AbstractTankAlgorithm.h"

#define SHOOTING_RANGE 8
namespace Algorithm_211388913_322330820
{
    // === Constructor === //
    AbstractTankAlgorithm::AbstractTankAlgorithm(int tank_idx, int player_idx)
        : tank_idx(tank_idx), player_idx(player_idx),
          sat_view(0, 0, 0, player_idx)

    {
        direction = ConfigReader::getConfig().getSpawnDirections()[player_idx - 1];
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

    void AbstractTankAlgorithm::setBoardHeight(int height)
    {
        this->height = height;
    }

    void AbstractTankAlgorithm::setBoardWidth(int width)
    {
        this->width = width;
    }

    // === Getters === //
    size_t AbstractTankAlgorithm::getRemainingShells() const { return num_of_shells; }
    BoardCell AbstractTankAlgorithm::getCurrentLocation() const { return assumed_location; }
    Direction AbstractTankAlgorithm::getTankDirection() const { return direction; }
    size_t AbstractTankAlgorithm::getCurrentStep() const { return current_step; }
    size_t AbstractTankAlgorithm::getTankIdx() const { return tank_idx; }
    GameObjectType AbstractTankAlgorithm::getTankType() const { return GameObjectTypeUtils::playerIndexToTankType(this->player_idx); }
    int AbstractTankAlgorithm::getBoardWidth() const { return width; };
    int AbstractTankAlgorithm::getBoardHeight() const { return height; };

    // === Cooldown / Wait Management === //
    bool AbstractTankAlgorithm::hasShells() const
    {
        return this->getRemainingShells() > 0;
    }
    bool AbstractTankAlgorithm::canTankShoot() const
    {
        return !cooldowns.isPendingBackwards() && cooldowns.isDoneShootCooldown() && hasShells();
    }

    bool AbstractTankAlgorithm::canMoveToCell(const BoardCell &cell) const
    {
        return !sat_view.isWallOnCell(cell);
    }
    void AbstractTankAlgorithm::handlePendingBackwards()
    {
        // this function moves the tank backwards on the end of cooldown.
        // if movement is not possible, cancel the backwards wait

        if (cooldowns.isBackwardsDue())
        {
            // if can move backwards - move
            BoardCell backwards_cell = this->createAdjustedBoardCell(this->getCurrentLocation() - this->getTankDirection());
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

        this->sat_view.applyApproxBoardChanges();
    }

    // === Action Planning === //
    ActionRequest AbstractTankAlgorithm::getTankEvasionAction(Direction chaser_direction) const
    {
        if (chaser_direction == this->getTankDirection())
        {
            if (auto opt_escape_cell = getEscapingRoute(chaser_direction))
            {
                return adjustDirection(opt_escape_cell.value());
            }
            return ActionRequest::Shoot;
        }
        else if (chaser_direction == DirectionUtils::getOppositeDirection(this->getTankDirection()))
        {
            if (auto opt_escape_cell = getEscapingRoute(chaser_direction))
            {
                return adjustDirection(opt_escape_cell.value());
            }
            return ActionRequest::Shoot;
        }
        else
        {
            return ActionRequest::MoveForward;
        }
    }

    ActionRequest AbstractTankAlgorithm::advanceTankToTarget(const BoardCell &target) const
    {
        BoardCell start = this->assumed_location;
        std::map<BoardCell, int> distances;
        std::map<BoardCell, BoardCell> parents;
        this->Dijkstra(this->getTankType(), start, target, distances, parents);

        // if cant reach target shoot and hope for good
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
        if (next_cell_in_path == this->getNextCellInDirection(start, this->getTankDirection()))
        {
            return ActionRequest::MoveForward;
        }
        if (next_cell_in_path == this->getNextCellInDirection(start, DirectionUtils::getOppositeDirection(this->getTankDirection())))
        {
            return ActionRequest::MoveBackward;
        }
        return this->adjustDirection(next_cell_in_path);
    }

    ActionRequest AbstractTankAlgorithm::adjustDirection(const BoardCell &to) const
    {
        // assumption: from, to are adjacent cells
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 1)))
        {
            return ActionRequest::RotateRight45;
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 2)))
        {
            return ActionRequest::RotateRight90;
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 3)))
        {
            return ActionRequest::RotateRight90; // will need additional 45 deg turn
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 1)))
        {
            return ActionRequest::RotateLeft45;
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 2)))
        {
            return ActionRequest::RotateLeft90;
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 3)))
        {
            return ActionRequest::RotateLeft90; // will need additional 45 deg turn
        }
        if (to == getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 4)))
        {
            return ActionRequest::RotateLeft90; // will need additional 90 deg turn
        }
        return ActionRequest::DoNothing; // in correct direction
    }

    // === Target Estimation === //
    BoardCell AbstractTankAlgorithm::approxClosestEnemyTankLocation() const
    {
        /*  this function gives an approximation of the closest enemy tank location.
            to be clear, the approximation is regarding the closest tank identity. the location is not an approximation.

            using direct distance to find distance in a fast way
        */
        BoardCell closest = this->assumed_location;
        int closest_dist = -1;
        for (BoardCell enemy_loc : sat_view.getEnemyTanksLocations())
        {
            int curr_dist = this->distance(this->assumed_location, enemy_loc);
            if (closest_dist == -1 || closest_dist > curr_dist)
            {
                closest_dist = curr_dist;
                closest = enemy_loc;
            }
        }
        return closest;
    }

    // === Pathfinding === //
    void AbstractTankAlgorithm::Dijkstra(GameObjectType tank_type, const BoardCell &start, const BoardCell &target, std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const
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
            for (BoardCell neighbor : getAdjacentCells(c))
            {
                if (neighbor == target || GameCollisionHandler::canObjectSafelyStepOn(sat_view, tank_type, neighbor))
                {

                    q.push({dist + 1, {neighbor, c}});
                }
            }
        }
    }

    // === Target Evaluation === //
    std::optional<ActionRequest> AbstractTankAlgorithm::evaluateShootingOpportunity(const BoardCell &target) const
    {
        BoardCell start = this->assumed_location;
        if (this->inShootRange(target) && this->canTankShoot())
        {
            if (isDirectionMatch(start, target, this->getTankDirection()))
                return ActionRequest::Shoot;

            else if (isStraightLine(start, target))
            {
                BoardCell adjust_to = getNextCellInStraightLine(start, target);
                return adjustDirection(adjust_to);
            }
            else
            { // Cell is not in direct range to target. can do nothing here.
            }
        }
        return std::nullopt;
    }

    std::optional<ActionRequest> AbstractTankAlgorithm::escapeShells() const
    {
        int step_dist = 5;
        step_dist = std::min(std::min(getBoardHeight(), getBoardWidth()), step_dist); // for small fields.
        int proximitiy = step_dist * ConfigReader::getConfig().getShellsSpeed();

        int left = this->assumed_location.getX() - proximitiy;
        int right = this->assumed_location.getX() + proximitiy;
        int up = this->assumed_location.getY() - proximitiy;
        int down = this->assumed_location.getY() + proximitiy;

        for (int x = left; x <= right; ++x)
        {
            for (int y = up; y <= down; ++y)
            {
                BoardCell cell = this->createAdjustedBoardCell(BoardCell(x, y));

                auto objects = sat_view.getObjectsAt(cell);
                for (auto [object_type_char, object_assumed_direction] : objects)
                {

                    if (object_type_char != static_cast<char>(GameObjectType::SHELL))
                        continue;

                    if (object_assumed_direction == AssumedDirection::UNKNOWN)
                        continue;

                    Direction obj_direction = static_cast<Direction>(object_assumed_direction);

                    if (isStraightLine(cell, this->assumed_location) &&
                        isDirectionMatch(cell, this->assumed_location, obj_direction))
                    {
                        return getTankEvasionAction(obj_direction);
                    }
                }
            }
        }

        return std::nullopt;
    }

    std::optional<BoardCell> AbstractTankAlgorithm::getEscapingRoute(Direction enemy_dir) const
    {
        std::vector<BoardCell> neighbors = getAdjacentCells(this->assumed_location);
        for (BoardCell neighbor : neighbors)
        {
            // check if stepped out from enemy direction
            if (isDirectionMatch(this->assumed_location, neighbor, enemy_dir) ||
                isDirectionMatch(this->assumed_location, neighbor, DirectionUtils::getOppositeDirection(enemy_dir)))
                continue;

            if (GameCollisionHandler::canObjectSafelyStepOn(sat_view, this->getTankType(), neighbor))
            {
                return neighbor;
            }
        }

        return std::nullopt;
    }

    //=== Interface === //
    ActionRequest AbstractTankAlgorithm::getAction()
    {
        this->advanceStep();
        ActionRequest request = this->getActionLogic();
        this->adjustSelfToAction(request);
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
            this->setBoardHeight(current_info.getBoardHeight());
            this->setBoardWidth(current_info.getBoardWidth());
        }

        // correction to location
        this->setCurrentLocation(current_info.getTankLocation());
        this->step_to_get_info = current_info.getStepToGetInfo(this->getCurrentStep());

        // get sat_view from BattleInfo
        this->sat_view = current_info.updateAndGetAnalyticsView(this->getCurrentStep(), this->sat_view);

        // update BattleInfo with algorithm data (tank_to_player_details)
        current_info.setCurrentStep(this->getCurrentStep());
        current_info.setTankDirection(this->direction);
        current_info.setRemainingShells(this->getRemainingShells());
    }

    //=== Helper Functions === //
    void AbstractTankAlgorithm::adjustSelfToAction(ActionRequest action)
    {
        // === Adjust self to action ===
        switch (action)
        {
        case ActionRequest::GetBattleInfo:
            this->adjustToGetBattleInfo();
            break;

        case ActionRequest::MoveForward:
            this->adjustToForward();
            break;

        case ActionRequest::MoveBackward:
            this->adjustToBackward();
            break;

        case ActionRequest::RotateLeft45:
        case ActionRequest::RotateLeft90:
        case ActionRequest::RotateRight45:
        case ActionRequest::RotateRight90:
            this->adjustToRotation(action);
            break;

        case ActionRequest::Shoot:
            this->adjustToShoot();
            break;

        case ActionRequest::DoNothing:
        default:
            break;
        }
    }
    
    void AbstractTankAlgorithm::adjustToRotation(ActionRequest rotation){
        if (!cooldowns.isPendingBackwards())
        {
            int steps = (rotation == ActionRequest::RotateLeft45 || rotation == ActionRequest::RotateRight45) ? 1 : 2;
            bool isLeft = (rotation == ActionRequest::RotateLeft45 || rotation == ActionRequest::RotateLeft90);
            this->direction = isLeft
                                  ? DirectionUtils::rotateLeft(this->direction, steps)
                                  : DirectionUtils::rotateRight(this->direction, steps);
        }
    }

    void AbstractTankAlgorithm::adjustToGetBattleInfo()
    {
        if (cooldowns.isPendingBackwards())
        {
            cooldowns.cancelBackwardWait();
        }
    }

    void AbstractTankAlgorithm::adjustToForward()
    {
        if (cooldowns.isPendingBackwards())
            {
                cooldowns.cancelBackwardWait();
            }
            else
            {

                BoardCell forward_cell = getNextCellInDirection(
                    this->assumed_location,
                    this->direction);

                if (canMoveToCell(forward_cell))
                {
                    this->setCurrentLocation(forward_cell);
                }
            }
    }

    void AbstractTankAlgorithm::adjustToBackward()
    {
        if (cooldowns.isPendingBackwards())
        {
            return;
        }
        else if (cooldowns.canImmediateBackwards())
        {
            BoardCell backward_cell = getNextCellInDirection(
                this->assumed_location,
                DirectionUtils::getOppositeDirection(this->direction));
            if (canMoveToCell(backward_cell))
            {
                this->assumed_location = backward_cell;
                cooldowns.extendBackwardsStreak();
            }
            else
            {
                cooldowns.cancelBackwardWait();
            }
        }
        else
        {
            cooldowns.startBackwardWait();
        }
    }

    void AbstractTankAlgorithm::adjustToShoot()
    {
        if (canTankShoot())
        {
            BoardCell shell_location = this->assumed_location + this->direction;
            this->sat_view.addShell(shell_location, this->direction);

            this->cooldowns.beginShoot();
            this->setRemainingShells(this->getRemainingShells() - 1);
        }
    }

    // === Manage Shooting Range === //
    bool AbstractTankAlgorithm::inShootRange(const BoardCell &cell) const
    {
        return this->distance(this->assumed_location, cell) <= SHOOTING_RANGE;
    }

    bool AbstractTankAlgorithm::isShellChasingTank(const BoardCell &shell_loc, AssumedDirection shell_assumed_dir) const
    {
        if (shell_assumed_dir == AssumedDirection::UNKNOWN)
        {
            return false;
        }

        Direction shell_dir = static_cast<Direction>(shell_assumed_dir);

        size_t dist = this->distance(this->assumed_location, shell_loc);
        if (dist <= ConfigReader::getConfig().getShellsSpeed() * 3 && isDirectionMatch(shell_loc, this->assumed_location, shell_dir))
        {
            return true;
        }
        return false;
    }

    // === Wrappers for GameBoardUtils Functions === //
    int AbstractTankAlgorithm::distance(const BoardCell &from, const BoardCell &to) const
    {
        return sat_view.distance(from, to);
    }

    BoardCell AbstractTankAlgorithm::createAdjustedBoardCell(const BoardCell &c) const
    {
        return sat_view.createAdjustedBoardCell(c);
    }

    BoardCell AbstractTankAlgorithm::getNextCellInDirection(const BoardCell &c, Direction dir) const
    {
        return sat_view.getNextCellInDirection(c, dir);
    }

    BoardCell AbstractTankAlgorithm::getNextCellInStraightLine(const BoardCell &from, const BoardCell &to) const
    {
        return sat_view.getNextCellInStraightLine(from, to);
    }

    bool AbstractTankAlgorithm::isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir) const
    {
        return sat_view.isDirectionMatch(from, to, dir);
    }

    std::vector<BoardCell> AbstractTankAlgorithm::getAdjacentCells(const BoardCell &cell) const
    {
        return sat_view.getAdjacentCells(cell);
    }

    bool AbstractTankAlgorithm::isStraightLine(const BoardCell &from, const BoardCell &to) const
    {
        return sat_view.isStraightLine(from, to);
    }
}