#pragma once
#include <array>

enum class GameObjectType
{
    TANK1 = '1',
    TANK2 = '2',
    WALL = '#',
    MINE = '@',
    SHELL = '*'
};

namespace GameObjectTypeUtils
{
    constexpr std::array<GameObjectType, 5> valid_types = {
        GameObjectType::TANK1,
        GameObjectType::TANK2,
        GameObjectType::WALL,
        GameObjectType::MINE,
        GameObjectType::SHELL
    };

    inline bool isValidObjectChar(char obj_char)
    {
        for (GameObjectType type : GameObjectTypeUtils::valid_types)
        {
            if (obj_char == static_cast<char>(type))
                return true;
        }
        return false;
    }
};