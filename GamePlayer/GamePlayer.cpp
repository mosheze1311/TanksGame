#include "GamePlayer.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include <queue>
#include <set>

Player::Player(GameObjectType tanks_type) : tanks_type(tanks_type) {}

GameObjectType Player::getEnemyTanksType()
{
    return this->tanks_type == GameObjectType::tank1 ? GameObjectType::tank2 : GameObjectType::tank1;
}

map<Tank *, TankAction> Player::getActionsFromPlayer(const GameBoard &board)
{
    map<Tank *, TankAction> res;

    vector<Tank *> player_tanks = board.getTanks(this->tanks_type);
    for (Tank *tank : player_tanks)
    {
        res[tank] = getTankAction(board, tank);
    }
    return res;
};
TankAction Player::player1Algorithm(const GameBoard &board, const Tank *tank)
{
    if (auto escape_shell_action = escapeShells(board, tank))
    {
        return escape_shell_action.value();
    }
    return TankAction::NOTHING;
}

TankAction Player::player2Algorithm(const GameBoard &board, const Tank *tank)
{
    /* aggressive algorithm using dijkstra
     */

    if (auto escape_shell_action = escapeShells(board, tank))
    {
        return escape_shell_action.value();
    }

    // for (Tank* enemy_tank : board.getTanks(this->getEnemyTanksType()))
    // {
    //     if(canEnemyKillTank(board, tank, enemy_tank)){
    //         return getEvasionAction(board, tank, enemy_tank);
    //     }
    // }

    return getAggressiveAction(board, tank);
}

optional<TankAction> Player::escapeShells(const GameBoard &board, const Tank *tank)
{
    // if there is a close bullet towards the tank or a tank that is in shooting range (and can shoot)
    for (GameObject *shell_object : board.getGameObjects(GameObjectType::shell))
    {
        Shell *shell = static_cast<Shell *>(shell_object);

        if (isShellChasingTank(board, tank, shell))
        {
            return getEvasionAction(board, tank, shell);
        }
    }
    return nullopt;
}
TankAction Player::getTankAction(const GameBoard &board, const Tank *tank)
{
    // select correct algorithm for each player
    if (this->tanks_type == GameObjectType::tank1)
    {
        return this->player1Algorithm(board, tank);
    }

    return player2Algorithm(board, tank);
}

TankAction Player::getEvasionAction(const GameBoard &board, const Tank *tank, const MovableObject *evade_from)
{
    // try to escape from tank/bullet

    // --- bullet escape ---
    // if can step out of trajectory, move
    // if has time to turn around and then escape, turn
    // if can shoot the bullet, shoot
    // otherwise accept your faith

    if (auto opt_cell = board.getObjectLocation(const_cast<Tank *>(tank)))
    {
        BoardCell c = opt_cell.value();

        if (evade_from->getDirection() == tank->getDirection())
        {
            if(auto opt_escape_cell = getEscapingRoute(board, tank, c, evade_from->getDirection())){
                return adjustDirection(board, c, opt_escape_cell.value(), tank->getDirection());
            }
            return TankAction::FIRE;
        }
        else if (evade_from->getDirection() == DirectionUtils::getOppositeDirection(tank->getDirection()))
        {
            if (auto opt_escape_cell = getEscapingRoute(board, tank, c, evade_from->getDirection()))
            {
                return adjustDirection(board, c, opt_escape_cell.value(), tank->getDirection());
            }
            return TankAction::FIRE;
        }
        else
        {
            return TankAction::FORWARD;
        }
    }

    // --- tank escape ---
    // (assuming can't shoot tank)
    // if can move from tank direction (sideways), move
    // if both in same direction and nothing in front, move
    // if facing each other and nothing behind, move backwards
    // otherwise accept your faith

    return TankAction::NOTHING; // should not get here - only if evade_from not on board.
}

optional<BoardCell> Player::getEscapingRoute(const GameBoard &board, const Tank *tank, BoardCell current_cell, Direction enemy_dir) const
{
    vector<BoardCell> neighbors = board.getAdjacentCells(current_cell);
    for (BoardCell neighbor : neighbors)
    {

        // check if stepped out from enemy direction
        if (isDirectionMatch(current_cell, neighbor, enemy_dir) ||
            isDirectionMatch(current_cell, neighbor, DirectionUtils::getOppositeDirection(enemy_dir)))
            continue;

        if (GameCollisionHandler::canObjectSafelyStepOn(board, tank->getObjectType(), neighbor))
        {
            return neighbor;
        }
    }

    return nullopt;
}

TankAction Player::getAggressiveAction(const GameBoard &board, const Tank *tank)
{
    // try to chase the enemy tank or shoot at it.

    // dijkstra to closest tank
    // if can shoot it - shoot
    // if can't shoot it - chase (try not to get into a bullet trajectory)
    optional<BoardCell> opt = board.getObjectLocation(const_cast<Tank *>(tank));
    if (!opt)
    {
        return TankAction::NOTHING; // should not happen
    }

    BoardCell start = opt.value();
    BoardCell target = this->approxClosestEnemyTankLocation(board, start);

    if (inShootRange(board, start, target) && tank->canShoot())
    {
        if (isDirectionMatch(start, target, tank->getDirection()))
            return TankAction::FIRE;
        else if (canAdjustDirection(start, target))
        {
            BoardCell adjust_to = getAdjustToCellTowardsTarget(start, target);
            return adjustDirection(board, start, adjust_to, tank->getDirection());
        }
        else
        { // nothing here
        }
    }

    return advanceTankToTarget(board, tank, start, target);
}

bool Player::inShootRange(const GameBoard &board, BoardCell from, BoardCell to)
{
    return board.distance(from, to) <= 8;
}
bool Player::isDirectionMatch(BoardCell from, BoardCell to, Direction dir) const
{
    if (!canAdjustDirection(from, to))
    {
        return false;
    }

    int x_diff = to.getX() - from.getX();
    int y_diff = to.getY() - from.getY();

    auto [dx, dy] = DirectionUtils::directionOffsets(dir);
    if (dx == 0 && x_diff == 0 && y_diff / dy > 0) // both up/down
    {
        return true;
    }
    if (dy == 0 && y_diff == 0 && x_diff / dx > 0) // both left/right
    {
        return true;
    }
    if (y_diff / dy == x_diff / dx && x_diff / dx > 0) // same 45 degrees
    {
        return true;
    }
    return false;
}

bool Player::canAdjustDirection(BoardCell from, BoardCell to) const
{
    int x_diff = to.getX() - from.getX();
    int y_diff = to.getY() - from.getY();

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

BoardCell Player::getAdjustToCellTowardsTarget(BoardCell from, BoardCell to) const
{
    int dx = to.getX() - from.getX();
    int dy = to.getY() - from.getY();
    int div = std::max(abs(dx), abs(dy));

    if (div == 0)
        return from;

    return BoardCell(from.getX() + dx / div, from.getY() + dy / div);
}

TankAction Player::adjustDirection(const GameBoard &board, BoardCell from, BoardCell to, Direction dir)
{
    // assumption: from, to are adjacent cells

    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 1)))
    {
        return TankAction::TURN45RIGHT;
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 2)))
    {
        return TankAction::TURN90RIGHT;
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 3)))
    {
        return TankAction::TURN90RIGHT; // will need additional 45 deg turn
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 1)))
    {
        return TankAction::TURN45LEFT;
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 2)))
    {
        return TankAction::TURN90LEFT;
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 3)))
    {
        return TankAction::TURN90LEFT; // will need additional 45 deg turn
    }
    if (to == board.getcreatedAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 4)))
    {
        return TankAction::TURN90LEFT; // will need additional 90 deg turn
    }

    return TankAction::NOTHING; // in correct direction
}

TankAction Player::advanceTankToTarget(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target)
{
    map<BoardCell, int> distances;
    map<BoardCell, BoardCell> parents;
    board.Dijkstra(start, target, distances, parents, tanks_type);

    // TODO: if cant reach target shoot and hope for good. maybe should do something smarter?
    if (distances.find(target) == distances.end())
    {
        return TankAction::FIRE;
    }

    // getting the next cell in the shortest path
    BoardCell next_cell_in_path = target;
    while (parents[next_cell_in_path] != start)
    {
        next_cell_in_path = parents[next_cell_in_path];
    }

    // decisions - try to advance to next cell
    if (next_cell_in_path == board.getcreatedAdjustedBoardCell(start + tank->getDirection()))
    {
        return TankAction::FORWARD;
    }
    if (next_cell_in_path == board.getcreatedAdjustedBoardCell(start - tank->getDirection()))
    {
        return TankAction::BACKWARD;
    }
    return adjustDirection(board, start, next_cell_in_path, tank->getDirection());
}

// TODO: think again about logic
bool Player::isShellChasingTank(const GameBoard &board, const Tank *tank, const Shell *shell)
{
    auto shell_opt_loc = board.getObjectLocation(const_cast<Shell *>(shell));
    auto tank_opt_loc = board.getObjectLocation(const_cast<Tank *>(tank));
    if (!shell_opt_loc || !tank_opt_loc)
    {
        return false;
    }
    BoardCell shell_loc = shell_opt_loc.value();
    BoardCell tank_loc = tank_opt_loc.value();

    int dist = board.distance(shell_loc, tank_loc);
    if (dist <= shell->getSpeed() * 3 && isDirectionMatch(shell_loc, tank_loc, shell->getDirection()))
    {
        return true;
    }
    return false;
}

bool Player::canEnemyKillTank(const GameBoard &board, const Tank *tank, const Tank *enemy)
{
    return false;
}


BoardCell Player::approxClosestEnemyTankLocation(const GameBoard &board, BoardCell start)
{
    /*  this function gives an approximation of the closest enemy tank location.
        to be clear, the approximation is regarding the closest tank identity. the location is not an approximation.

        using direct distance to find distance in a fast way
    */
    BoardCell closest = start;
    int closest_dist = -1;
    for (Tank *enemy : board.getTanks(this->getEnemyTanksType()))
    {
        auto opt_loc = board.getObjectLocation(enemy);
        if (!opt_loc)
            continue;
        BoardCell enemy_loc = opt_loc.value();

        int curr_dist = board.distance(closest, enemy_loc);
        if (closest_dist == -1 || closest_dist > curr_dist)
        {
            closest_dist = curr_dist;
            closest = enemy_loc;
        }
    }
    return closest;
}