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


};
