#pragma once
#include "../common/TankAlgorithm.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../SatelliteView/BoardSatelliteView.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"

#include <queue>
#include <set>

class AbstractTankAlgorithm : public TankAlgorithm
{

private:

    void Dijkstra(const SatelliteAnalyitcsView& sat_view, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents)
    {
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

            // adding neighbors to heap - only if tank can safely step there
            for (BoardCell neighbor : GameBoardUtils::getAdjacentCells(c, sat_view.getWidth(), sat_view.getHeight()))
            {
                if (visited.find(neighbor) != visited.end())
                {
                    continue;
                }

                if (neighbor == target || GameCollisionHandler::canObjectSafelyStepOn(sat_view, tank_type, neighbor))
                {
                    q.push({dist + 1, {neighbor, c}});
                }
            }
        }
    }

    ActionRequest getTankEvasionAction(const  SatelliteView& sat_view, const Tank *tank, const MovableObject *evade_from) const
 {
//     // try to escape from tank/bullet

//     // --- bullet escape ---
//     // if can step out of trajectory, move
//     // if has time to turn around and then escape, turn
//     // if can shoot the bullet, shoot
//     // otherwise accept your faith

//     if (auto opt_cell = board.getObjectLocation(tank))
//     {
//         BoardCell c = opt_cell.value();

//         if (evade_from->getDirection() == tank->getDirection())
//         {
//             if (auto opt_escape_cell = getEscapingRoute(board, tank, c, evade_from->getDirection()))
//             {
//                 return adjustDirection(board, c, opt_escape_cell.value(), tank->getDirection());
//             }
//             return TankAction::FIRE;
//         }
//         else if (evade_from->getDirection() == DirectionUtils::getOppositeDirection(tank->getDirection()))
//         {
//             if (auto opt_escape_cell = getEscapingRoute(board, tank, c, evade_from->getDirection()))
//             {
//                 return adjustDirection(board, c, opt_escape_cell.value(), tank->getDirection());
//             }
//             return TankAction::FIRE;
//         }
//         else
//         {
//             return TankAction::FORWARD;
//         }
//     }

//     // --- tank escape ---
//     // (assuming can't shoot tank)
//     // if can move from tank direction (sideways), move
//     // if both in same direction and nothing in front, move
//     // if facing each other and nothing behind, move backwards
//     // otherwise accept your faith

//     return TankAction::NOTHING; // should not get here - only if evade_from not on board.
 }


ActionRequest getTankAggressiveAction(const  SatelliteView& sat_view, const Tank *tank) const
{
    // // try to chase the enemy tank or shoot at it.

    // // dijkstra to closest tank
    // // if can shoot it - shoot
    // // if can't shoot it - chase
    // optional<BoardCell> opt = board.getObjectLocation(tank);
    // if (!opt)
    // {
    //     return TankAction::NOTHING; // should not happen
    // }

    // BoardCell start = opt.value();
    // BoardCell target = this->approxClosestEnemyTankLocation(board, start);

    // // try to shoot enemy
    // if(auto shoot_action_opt = attemptShoot(board, tank, start, target)){
    //     return shoot_action_opt.value();
    // }
    
    // // if will be able to shoot in the future, wait 
    // if (PlayerUtils::inShootRange(board, start, target) && board.isStraightLine(start, target) && tank->getShells() > 0)
    // {
    //     return TankAction::NOTHING;
    // }
    // return advanceTankToTarget(board, tank, start, target);
}

};
