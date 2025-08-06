#pragma once

#include "../GameObjects/GameObjectType.h"

#include <array>
#include <iostream> // TODO: delete include
namespace UserCommon_211388913_322330820
{

    class GameObjectTypeUtils final
    {
    private:
        // === Attributes === //
        inline static const std::array<GameObjectType, 12> valid_types = {
            GameObjectType::TANK1,
            GameObjectType::TANK2,
            GameObjectType::WALL,
            GameObjectType::MINE,
            GameObjectType::SHELL,
        };

        // === Constructor === //
        GameObjectTypeUtils() = delete;

    public:
        static bool isValidObjectChar(char obj_char)
        {
            for (GameObjectType type : GameObjectTypeUtils::valid_types)
            {
                if (obj_char == static_cast<char>(type))
                    return true;
            }
            return false;
        }

        static int tankTypeToPlayerIndex(GameObjectType type)
        {
            char playerChar = static_cast<char>(type);
            int playerId = playerChar - '0';

            return playerId;
        }

        static GameObjectType playerIndexToTankType(int playerId)
        {
            int wrappedId = ((playerId - 1) % 9 + 9) % 9;
            char playerChar = static_cast<char>('1' + wrappedId);
            return static_cast<GameObjectType>(playerChar);
        }

        static bool isTankObject(GameObjectType t)
        {
            switch (t)
            {
            case GameObjectType::TANK1:
            case GameObjectType::TANK2:
                return true;
            default:
                return false;
            }
        }

        static bool isAllyTank(char tank_char, int player_idx)
        {
            return tank_char == static_cast<char>(playerIndexToTankType(player_idx));
        }
    };
}