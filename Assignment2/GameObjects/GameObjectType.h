#pragma once
#include <array>

enum class GameObjectType
{
    TANK1 = '1',
    TANK2 = '2',
    TANK3 = '3',
    TANK4 = '4',
    TANK5 = '5',
    TANK6 = '6',
    TANK7 = '7',
    TANK8 = '8',
    TANK9 = '9',
    WALL = '#',
    MINE = '@',
    SHELL = '*',

};

namespace GameObjectTypeUtils
{
    constexpr std::array<GameObjectType, 12> valid_types = {
        GameObjectType::TANK1,
        GameObjectType::TANK2,
        GameObjectType::TANK3,
        GameObjectType::TANK4,
        GameObjectType::TANK5,
        GameObjectType::TANK6,
        GameObjectType::TANK7,
        GameObjectType::TANK8,
        GameObjectType::TANK9,

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

    inline int tankTypeToPlayerIndex(GameObjectType type)
    {
        char playerChar = static_cast<char>(type);  
        int playerId = playerChar - '0';      
        return playerId;  
    }

    inline GameObjectType playerIndexToTankType(int playerId)
    {
    int wrappedId = ((playerId - 1) % 9 + 9) % 9;
    char playerChar = static_cast<char>('1' + wrappedId);
    return static_cast<GameObjectType>(playerChar);
    }

    inline bool isTankObject(GameObjectType t){
        switch (t)
        {
            case GameObjectType::TANK1:
            case GameObjectType::TANK2:
            case GameObjectType::TANK3:
            case GameObjectType::TANK4:
            case GameObjectType::TANK5:
            case GameObjectType::TANK6:
            case GameObjectType::TANK7:
            case GameObjectType::TANK8:
            case GameObjectType::TANK9:
                return true;
            default:
                return false;
        }
    }

};