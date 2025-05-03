#pragma once
#include <utility>

enum class Direction
{
    UP = 0,
    UPR = 1,
    RIGHT = 2,
    DOWNR = 3,
    DOWN = 4,
    DOWNL = 5,
    LEFT = 6,
    UPL = 7
    
};

namespace DirectionUtils
{

    constexpr inline Direction rotateRight(Direction dir, int change)
    {
        int curr_dir = static_cast<int>(dir);
        int new_dir = (curr_dir + change) % 8;
        return static_cast<Direction>(new_dir);
    }

    constexpr inline Direction rotateLeft(Direction dir, int change)
    {
        int curr_dir = static_cast<int>(dir);
        int new_dir = (curr_dir - change + 8) % 8;
        return static_cast<Direction>(new_dir);
    }

    constexpr inline std::pair<int, int> directionOffsets(Direction dir)
    {
        switch (dir)
        {
        case Direction::UP:
            return {0, -1};
        case Direction::UPR:
            return {1, -1};
        case Direction::RIGHT:
            return {1, 0};
        case Direction::DOWNR:
            return {1, 1};
        case Direction::DOWN:
            return {0, 1};
        case Direction::DOWNL:
            return {-1, 1};
        case Direction::LEFT:
            return {-1, 0};
        case Direction::UPL:
            return {-1, -1};
        default:
            // This should never happen
            return {0, 0};
        }
    }

    constexpr inline Direction getOppositeDirection(Direction dir){
        return rotateRight(dir, 4);
    }
}