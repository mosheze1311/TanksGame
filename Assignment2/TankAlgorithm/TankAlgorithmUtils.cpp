#include "../config.h"

#include "TankAlgorithmUtils.h"

#include <set>
#include <queue>
#include <limits>
#include <utility>
#define SHOOTING_RANGE 8

bool TankAlgorithmUtils::inShootRange(BoardCell from, BoardCell to, size_t width, size_t height)
{
    return GameBoardUtils::distance(from, to, width, height) <= SHOOTING_RANGE;
}

bool TankAlgorithmUtils::isShellChasingTank(BoardCell shell_loc, AssumedDirection shell_assumed_dir ,BoardCell tank_loc, size_t width, size_t height)
{
    // TODO: address this if with better logic :)
    if (shell_assumed_dir == AssumedDirection::UNKNOWN){
        return false;
    }

    Direction shell_dir = static_cast<Direction>(shell_assumed_dir);

    int dist = GameBoardUtils::distance(shell_loc, tank_loc, width, height);
    if (dist <= shell_speed * 3 && GameBoardUtils::isDirectionMatch(shell_loc, tank_loc, shell_dir, width, height))
    {
        return true;
    }
    return false;
}
