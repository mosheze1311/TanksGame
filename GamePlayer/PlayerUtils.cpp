#include "PlayerUtils.h"

#include <set>
#include <queue>
#include <limits>
#include <utility>
#define SHOOTING_RANGE 8

// Dijkstra's algorithm to find shortest paths from a source node
void PlayerUtils::Dijkstra(const GameBoard &board, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents)
{
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

        // adding neighbors to heap - only if tank can safely step there
        for (BoardCell neighbor : board.getAdjacentCells(c))
        {
            if (target == neighbor || GameCollisionHandler::canObjectSafelyStepOn(board, tank_type, neighbor))
            {
                q.push({dist + 1, {neighbor, c}});
            }
        }
    }
}

bool PlayerUtils::inShootRange(const GameBoard &board, BoardCell from, BoardCell to)
{
    return board.distance(from, to) <= SHOOTING_RANGE;
}


BoardCell PlayerUtils::getNextCellInStraightLine(BoardCell from, BoardCell to)
{
    // assuming that from, to form a straight line in some direction.
    int dx = to.getX() - from.getX();
    int dy = to.getY() - from.getY();
    int div = std::max(abs(dx), abs(dy));

    if (div == 0)
        return from;

    return BoardCell(from.getX() + dx / div, from.getY() + dy / div);
}

// TODO: think again about logic
bool PlayerUtils::isShellChasingTank(const GameBoard &board, const Tank *tank, const Shell *shell)
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
    if (dist <= shell->getSpeed() * 3 && board.isDirectionMatch(shell_loc, tank_loc, shell->getDirection()))
    {
        return true;
    }
    return false;
}
