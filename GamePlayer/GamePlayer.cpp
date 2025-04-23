#include "GamePlayer.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include <queue>
#include <set>
#include "iostream"

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

TankAction Player::getTankAction(const GameBoard &board, const Tank *tank)
{
    // TODO: this is temp to test the dijkstra, keep in same place
    if(this->tanks_type == GameObjectType::tank1){
        return TankAction::NOTHING;
    }
    
    // if there is a close bullet towards the tank or a tank that is in shooting range (and can shoot)
    for (GameObject *shell_object : board.getGameObjects(GameObjectType::shell))
    {
        Shell *shell = static_cast<Shell *>(shell_object);

        if (isShellChasingTank(board, tank, shell))
        {
            return getEvasionAction(board, tank, shell);
        }
    }

    // for (Tank* enemy_tank : board.getTanks(this->getEnemyTanksType()))
    // {
    //     if(canEnemyKillTank(board, tank, enemy_tank)){
    //         return getEvasionAction(board, tank, enemy_tank);
    //     }
    // }

    return getAggressiveAction(board, tank);
}

// TODO: write this function
TankAction Player::getEvasionAction(const GameBoard &board, const Tank *tank, const MovableObject *evade_from)
{
    // try to escape from tank/bullet

    // --- bullet escape ---
    // if can step out of trajectory, move
    // if has time to turn around and then escape, turn
    // if can shoot the bullet, shoot
    // otherwise accept your faith

    // --- tank escape ---
    // (assuming can't shoot tank)
    // if can move from tank direction (sideways), move
    // if both in same direction and nothing in front, move
    // if facing each other and nothing behind, move backwards
    // otherwise accept your faith

    bool turn = std::rand() % 4 == 0;
    return turn ? TankAction::TURN45RIGHT : TankAction::FORWARD;
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

    if (inShootRange(board ,start, target) && tank->canShoot())
    {
        if (isDirectionMatch(start, target, tank->getDirection()))
            return TankAction::FIRE;
            else if (canAdjustDirection(start, target)) {
                BoardCell adjust_to = getAdjustToCellTowardsTarget(start, target);
                return adjustDirection(board, start, adjust_to, tank->getDirection());
            }
        else
        { // nothing here
        }
    }

    return advanceTankToTarget(board, tank, start, target);
}


bool Player::inShootRange(const GameBoard &board,BoardCell from, BoardCell to)
{
    return board.distance(from, to) <= 8;
}
bool Player::isDirectionMatch(BoardCell from, BoardCell to, Direction dir)
{
    if(!canAdjustDirection(from, to)){
        return false;
    }

    int x_diff = to.getX() - from.getX();
    int y_diff = to.getY() - from.getY();

    auto [dx, dy] = DirectionUtils::directionOffsets(dir);
    if (dx == 0 && x_diff == 0 && y_diff/dy >0) // both up/down
    {
        return true;
    }
    if (dy == 0 && y_diff == 0 && x_diff / dx > 0) // both left/right
    {
        return true;
    }
    if(y_diff/dy == x_diff/dx && x_diff/dx > 0) // same 45 degrees
    {
        return true;
    }
    return false;
}


bool Player::canAdjustDirection(BoardCell from, BoardCell to)
{
    int x_diff = to.getX() - from.getX();
    int y_diff = to.getY() - from.getY();

    if(x_diff == 0 || y_diff == 0){
        return true;
    } 
    if (abs(x_diff) == abs(y_diff)){
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

TankAction Player::adjustDirection(const GameBoard& board, BoardCell from, BoardCell to, Direction dir)
{
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 1)))
    {
        return TankAction::TURN45RIGHT;
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 2)))
    {
        return TankAction::TURN90RIGHT;
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateRight(dir, 3)))
    {
        return TankAction::TURN90RIGHT; // will need additional 45 deg turn
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 1)))
    {
        return TankAction::TURN45LEFT;
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 2)))
    {
        return TankAction::TURN90LEFT;
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 3)))
    {
        return TankAction::TURN90LEFT; // will need additional 45 deg turn
    }
    if (to == board.createAdjustedBoardCell(from + DirectionUtils::rotateLeft(dir, 4)))
    {
        return TankAction::TURN90LEFT; // will need additional 90 deg turn
    }

    return TankAction::NOTHING; // in correct direction
}

TankAction Player::advanceTankToTarget(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target)
{
    map<BoardCell, int> distances;
    map<BoardCell, BoardCell> parents;
    Dijkstra(board, start, target,distances, parents);

    //TODO: if cant reach target shoot and hope for good. maybe should do something smarter?
    if(distances.find(target) == distances.end()){
        return TankAction::FIRE;
    }

    // getting the next cell in the shortest path
    BoardCell next_cell_in_path = target;
    while (parents[next_cell_in_path] != start){
        next_cell_in_path = parents[next_cell_in_path];
    }

    // decisions - try to advance to next cell
    if (next_cell_in_path == board.createAdjustedBoardCell(start + tank->getDirection()))
    {
        return TankAction::FORWARD;
    }
    if (next_cell_in_path == board.createAdjustedBoardCell(start - tank->getDirection()))
    {
        return TankAction::BACKWARD;
    }
    return adjustDirection(board, start,next_cell_in_path,tank->getDirection());
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
    int next_dist = board.distance(tank_loc, shell_loc + shell->getDirection());
    if (dist <= shell->getSpeed() && next_dist < dist)
    {
        return true;
    }
    return false;
}

bool Player::canEnemyKillTank(const GameBoard &board, const Tank *tank, const Tank *enemy)
{
    return false;
}

void Player::Dijkstra(const GameBoard &board, BoardCell start, BoardCell target, map<BoardCell,int>& distances, map<BoardCell,BoardCell>& parents)
{
    // TODO: Currently implemented as Player method. Decide on a better class (Algorithms, Board, PlayerStrategies, PlayerUtils)?
    // TODO: Currently behaves like BFS. Consider using different weights when cell require turning.
    // TODO: Currently scanning entire board. Consider improving to A* like algorithm.

    priority_queue<
        pair<int, pair<BoardCell, BoardCell>>,
        vector<pair<int, pair<BoardCell, BoardCell>>>,
        greater<pair<int, pair<BoardCell, BoardCell>>>>
        q;
    q.push({0, {start, start}});
    set<BoardCell> visited;
    
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

        // adding neighbours to heap - only if tank can safely step there
        for (int dir_number = 0; dir_number < 8; dir_number++)
        {
            BoardCell neighbour = board.createAdjustedBoardCell(c + static_cast<Direction>(dir_number));
            if (target == neighbour || GameCollisionHandler::canObjectSafelyStepOn(board, this->tanks_type, neighbour))
            {
                q.push({dist + 1, {neighbour, c}});
            }
        }
    }
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