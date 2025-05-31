#pragma once

#include "../GameObjects/Direction.h"

#include <utility>

namespace DirectionUtils
{

    // === Rotation Functions === //
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

    constexpr inline Direction getOppositeDirection(Direction dir)
    {
        return rotateRight(dir, 4);
    }

    // === Offset Managment === //
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
        };
    }

    // === Operators === //
    constexpr inline std::pair<int, int> operator*(int times, Direction dir)
    {
        std::pair<int, int> offsets = directionOffsets(dir);
        offsets.first *= times;
        offsets.second *= times;
        return offsets;
    }

    constexpr inline std::pair<int, int> operator*(Direction dir, int times)
    {
        return times * dir;
    }
}