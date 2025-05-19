#pragma once
#include "../common/TankAlgorithm.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../SatelliteView/BoardSatelliteView.h"

#include <queue>
#include <set>

class AbstractTankAlgorithm : public TankAlgorithm
{

private:
    void Dijkstra(const  SatelliteView& sat_view, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents)
    {
        // priority_queue<
        //     pair<int, pair<BoardCell, BoardCell>>,
        //     vector<pair<int, pair<BoardCell, BoardCell>>>,
        //     greater<pair<int, pair<BoardCell, BoardCell>>>>
        //     q;
        // q.push({0, {start, start}});
        // set<BoardCell> visited;

        // while (!q.empty())
        // {
        //     // access and pop first item in heap
        //     auto [dist, current_and_parent_pair] = q.top();
        //     auto [c, parent] = current_and_parent_pair;
        //     q.pop();

        //     // skip visited since inserting cells multiple times
        //     if (visited.find(c) != visited.end())
        //     {
        //         continue;
        //     }

        //     // mark visted and save distance, parent data
        //     visited.insert(c);
        //     parents[c] = parent;
        //     distances[c] = dist;

        //     // early termination when reaching target
        //     if (c == target)
        //         break;

        //     // adding neighbors to heap - only if tank can safely step there
        //     for (BoardCell neighbor : GameBoardUtils::getAdjacentCells(c, , ))
        //     {
        //         if (target == neighbor || GameCollisionHandler::canObjectSafelyStepOn(board, tank_type, neighbor))
        //         {
        //             q.push({dist + 1, {neighbor, c}});
        //         }
        //     }
        // }
    }

// TankAction Player::getTankEvasionAction(const GameBoard &board, const Tank *tank, const MovableObject *evade_from) const
// {
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
// }


};
