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

    // === Getters === //
    size_t AbstractTankAlgorithm::getRemainingShells() const { return num_of_shells; }
    BoardCell AbstractTankAlgorithm::getCurrentLocation() const { return assumed_location; }
    Direction AbstractTankAlgorithm::getTankDirection() const { return direction; }
    size_t AbstractTankAlgorithm::getCurrentStep() const { return current_step; }
    size_t AbstractTankAlgorithm::getTankIdx() const { return tank_idx; }
    GameObjectType AbstractTankAlgorithm::getTankType() const { return GameObjectTypeUtils::playerIndexToTankType(this->player_idx); }
    int AbstractTankAlgorithm::getBoardWidth() const { return sat_view.getWidth(); };
    int AbstractTankAlgorithm::getBoardHeight() const { return sat_view.getHeight(); };

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
            BoardCell backwards_cell = sat_view.createAdjustedBoardCell(this->getCurrentLocation() - this->getTankDirection());
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
    std::set<BoardCell> AbstractTankAlgorithm::getAlliesReservedCells() const
    {
        std::set<BoardCell> reserved_cells;
        for (const BoardCell &neighbor_cell : sat_view.getAdjacentCells(getCurrentLocation()))
        {
            for (const BoardCell &ally_location : sat_view.getAlliesTanksLocations())
            {
                if (sat_view.distance(ally_location, neighbor_cell) <= 1)
                {
                    reserved_cells.insert(neighbor_cell);
                }
            }
        }
        return reserved_cells;
    }
    std::optional<BoardCell> AbstractTankAlgorithm::findNextCellInPath(const BoardCell &start, const BoardCell &target) const
    {
        std::map<BoardCell, int> distances;
        std::map<BoardCell, BoardCell> parents;
        std::set<BoardCell> allies_reserved = getAlliesReservedCells();
        this->Dijkstra(start, target, allies_reserved, distances, parents);

        // if cant reach target shoot and hope for good
        if (distances.find(target) == distances.end())
        {
            return std::nullopt;
        }

        // getting the next cell in the shortest path
        BoardCell next_cell_in_path = target;
        while (parents[next_cell_in_path] != start)
        {
            next_cell_in_path = parents[next_cell_in_path];
        }

        return next_cell_in_path;
    }
    ActionRequest AbstractTankAlgorithm::advanceTankToTarget(const BoardCell &target) const
    {
        BoardCell start = this->assumed_location;
        auto next_cell_opt = findNextCellInPath(start, target);
        if (next_cell_opt == std::nullopt)
            return ActionRequest::Shoot;

        BoardCell next_cell_in_path = *next_cell_opt;
        // decisions - try to advance to next cell
        if (next_cell_in_path == sat_view.getNextCellInDirection(start, this->getTankDirection()))
        {
            return ActionRequest::MoveForward;
        }
        if (next_cell_in_path == sat_view.getNextCellInDirection(start, DirectionUtils::getOppositeDirection(this->getTankDirection())))
        {
            return ActionRequest::MoveBackward;
        }
        return this->adjustDirection(next_cell_in_path);
    }

    ActionRequest AbstractTankAlgorithm::adjustDirection(const BoardCell &to) const
    {
        // assumption: from, to are adjacent cells
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 1)))
        {
            return ActionRequest::RotateRight45;
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 2)))
        {
            return ActionRequest::RotateRight90;
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateRight(this->getTankDirection(), 3)))
        {
            return ActionRequest::RotateRight90; // will need additional 45 deg turn
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 1)))
        {
            return ActionRequest::RotateLeft45;
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 2)))
        {
            return ActionRequest::RotateLeft90;
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 3)))
        {
            return ActionRequest::RotateLeft90; // will need additional 45 deg turn
        }
        if (to == sat_view.getNextCellInDirection(this->assumed_location, DirectionUtils::rotateLeft(this->getTankDirection(), 4)))
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
            int curr_dist = sat_view.distance(this->assumed_location, enemy_loc);
            if (closest_dist == -1 || closest_dist > curr_dist)
            {
                closest_dist = curr_dist;
                closest = enemy_loc;
            }
        }
        return closest;
    }

    // === Pathfinding === //
    void AbstractTankAlgorithm::Dijkstra(const BoardCell &start, const BoardCell &target, const std::set<BoardCell> &forbidden, std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const
    {
        struct DijkstraStatus
        {
        public:
            BoardCell current;
            Direction dir;
            BoardCell parent;
            bool operator<(const DijkstraStatus &other) const
            {
                return std::tie(current, dir, parent) < std::tie(other.current, other.dir, other.parent);
            };
        };
        std::priority_queue<
            std::pair<int, DijkstraStatus>,
            std::vector<std::pair<int, DijkstraStatus>>,
            std::greater<std::pair<int, DijkstraStatus>>>
            q;
        q.push({0, {start, this->getTankDirection(), start}});
        std::set<BoardCell> visited;
        while (!q.empty())
        {
            // access and pop first item in heap
            auto [dist, details] = q.top();
            BoardCell c = details.current;
            BoardCell parent = details.parent;
            Direction dir = details.dir;
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
            int dirs_count = 8;
            for (int new_dir_num = 0; new_dir_num < dirs_count; ++new_dir_num)
            {
                Direction new_dir = static_cast<Direction>(new_dir_num);
                BoardCell neighbor = c + new_dir;
                if (forbidden.contains(neighbor))
                    continue;

                int diff = new_dir_num - static_cast<int>(dir);
                int rotation_cost = (5 - std::abs(std::abs(diff) - 4)) / 2; // formula gives amount of rotations needed
                if (neighbor == target || GameCollisionHandler::canObjectSafelyStepOn(sat_view, this->getTankType(), neighbor))
                {
                    q.push({dist + rotation_cost + 1, {neighbor, new_dir, c}});
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
            if (sat_view.isDirectionMatch(start, target, this->getTankDirection()))
            {
                int dist_to_target = sat_view.distance(assumed_location, target);
                for (BoardCell ally : sat_view.getAlliesTanksLocations())
                {
                    if (sat_view.isDirectionMatch(assumed_location, ally, this->getTankDirection()) && 
                        sat_view.distance(assumed_location, ally) < dist_to_target)
                        {
                        return std::nullopt;
                        }
                }
                return ActionRequest::Shoot;
            }

            else if (sat_view.isStraightLine(start, target))
            {
                BoardCell adjust_to = sat_view.getNextCellInStraightLine(start, target);
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

        auto [left, right, up, down] = sat_view.proximityBoundaries(this->assumed_location, proximitiy);
        for (int x = left; x <= right; ++x)
        {
            for (int y = up; y <= down; ++y)
            {
                BoardCell cell = sat_view.createAdjustedBoardCell(BoardCell(x, y));

                auto objects = sat_view.getObjectsAt(cell);
                for (auto [object_type_char, object_assumed_direction] : objects)
                {

                    if (object_type_char != static_cast<char>(GameObjectType::SHELL))
                        continue;

                    if (object_assumed_direction == AssumedDirection::UNKNOWN)
                        continue;

                    Direction obj_direction = static_cast<Direction>(object_assumed_direction);

                    if (sat_view.isStraightLine(cell, this->assumed_location) &&
                        sat_view.isDirectionMatch(cell, this->assumed_location, obj_direction))
                    {
                        return getTankEvasionAction(obj_direction);
                    }
                }
            }
        }

        return std::nullopt;
    }

    bool AbstractTankAlgorithm::isShellApproaching() const
    {
        int step_dist = 5;
        step_dist = std::min(std::min(getBoardHeight(), getBoardWidth()), step_dist); // for small fields.
        int proximitiy = step_dist * ConfigReader::getConfig().getShellsSpeed();

        auto [left, right, up, down] = sat_view.proximityBoundaries(this->assumed_location, proximitiy);

        for (int x = left; x <= right; ++x)
        {
            for (int y = up; y <= down; ++y)
            {
                BoardCell cell = sat_view.createAdjustedBoardCell(BoardCell(x, y));
                if (this->sat_view.getObjectsTypesOnCell(cell).contains(GameObjectType::SHELL))
                {
                    AssumedDirection shell_direction = *(sat_view.getDirectionOfObjectAt(GameObjectType::SHELL, cell));

                    if (shell_direction == AssumedDirection::UNKNOWN)
                        return true;

                    int distance = sat_view.distance(cell, this->assumed_location);
                    BoardCell next_shell_location = sat_view.createAdjustedBoardCell(cell + static_cast<Direction>(shell_direction));
                    int next_distance = sat_view.distance(next_shell_location, this->assumed_location);
                    if (distance > next_distance)
                        return true;
                }
            }
        }
        return false;
    }

    std::optional<BoardCell> AbstractTankAlgorithm::getEscapingRoute(Direction enemy_dir) const
    {
        std::vector<BoardCell> neighbors = sat_view.getAdjacentCells(this->assumed_location);
        for (BoardCell neighbor : neighbors)
        {
            // check if stepped out from enemy direction
            if (sat_view.isDirectionMatch(this->assumed_location, neighbor, enemy_dir) ||
                sat_view.isDirectionMatch(this->assumed_location, neighbor, DirectionUtils::getOppositeDirection(enemy_dir)))
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
        BattleInfoAgent &current_info = dynamic_cast<BattleInfoAgent &>(info);

        // if first time getting battle info - must init some of the details from it.
        if (this->getCurrentStep() == 1)
        {
            // set game settings and save them
            this->setRemainingShells(current_info.getInitialNumShells());
            this->setMaxSteps(current_info.getMaxSteps());
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

    void AbstractTankAlgorithm::adjustToRotation(ActionRequest rotation)
    {
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

            BoardCell forward_cell = sat_view.getNextCellInDirection(
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
            BoardCell backward_cell = sat_view.getNextCellInDirection(
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
        return sat_view.distance(this->assumed_location, cell) <= SHOOTING_RANGE;
    }

    bool AbstractTankAlgorithm::isShellInTankDirection(const BoardCell &shell_loc, AssumedDirection shell_assumed_dir) const
    {
        if (shell_assumed_dir == AssumedDirection::UNKNOWN)
        {
            return false;
        }

        Direction shell_dir = static_cast<Direction>(shell_assumed_dir);

        size_t dist = sat_view.distance(this->assumed_location, shell_loc);
        if (dist <= ConfigReader::getConfig().getShellsSpeed() * 3 &&
            sat_view.isDirectionMatch(shell_loc, this->assumed_location, shell_dir))
        {
            return true;
        }
        return false;
    }

    // === Aggresive Algorithm === //
    ActionRequest AbstractTankAlgorithm::getTankAggressiveAction() const
    {
        // try to chase the enemy tank or shoot at it.

        // dijkstra to closest tank
        // if can shoot it - shoot
        // if can't shoot it - chase

        BoardCell start = this->assumed_location;
        BoardCell target = this->approxClosestEnemyTankLocation();

        // try to shoot enemy
        if (auto shoot_action_opt = this->evaluateShootingOpportunity(target))
        {
            return shoot_action_opt.value();
        }

        // if will be able to shoot in the future, wait
        if (this->inShootRange(target) &&
            sat_view.isStraightLine(start, target) && this->hasShells())
        {
            return ActionRequest::GetBattleInfo;
        }

        return advanceTankToTarget(target);
    }

    // === Survival Tactic === //
    std::pair<ActionRequest, float> AbstractTankAlgorithm::getBestProbSurvivalAction(const AbstractTankAlgorithm &algo,
                                                                                     int steps_to_calculate, ActionRequest default_action)
    {
        if (steps_to_calculate == 0)
            return {ActionRequest::GetBattleInfo, 0};

        std::map<ActionRequest, float> evasion_actions_scores;
        for (ActionRequest action : ActionRequestUtils::getAllRequests())
        {
            evasion_actions_scores[action] = AbstractTankAlgorithm::bestScoreFor(algo, action, steps_to_calculate);
        }

        // find best action and its score
        ActionRequest best_action = default_action;
        float max_score = evasion_actions_scores[default_action];

        for (const auto &[action, score] : evasion_actions_scores)
        {
            if (score > max_score)
            {
                max_score = score;
                best_action = action;
            }
        }

        return {best_action, max_score};
    }

    float AbstractTankAlgorithm::bestScoreFor(const AbstractTankAlgorithm &algo,
                                              ActionRequest action,
                                              int steps_to_calculate)
    {
        // copy of the analytics view
        std::unique_ptr<AbstractTankAlgorithm> algo_copy = algo.clone();

        // simmulate action and step advancement on algo_copy
        algo_copy->adjustSelfToAction(action);
        bool is_dead = !GameCollisionHandler::canObjectSafelyStepOn(algo_copy->sat_view, GameObjectTypeUtils::playerIndexToTankType(algo_copy->player_idx), algo_copy->getCurrentLocation());
        if (is_dead)
            return 0;
        algo_copy->sat_view.setMonitoredCell(algo_copy->getCurrentLocation());
        algo_copy->advanceStep();
        is_dead = is_dead || algo_copy->sat_view.isMonidoredCellHit();

        // calculate score for action
        float score = is_dead ? 0 : 1;
        if (!is_dead)
        {
            score += 0.7 * getBestProbSurvivalAction(*algo_copy, steps_to_calculate - 1, ActionRequest::GetBattleInfo).second;
        }
        return score;
    }
}



