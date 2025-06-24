#pragma once

#include "../GameObjects/GameObjectType.h"

#include <array>
namespace UserCommon_211388913_322330820
{

    class GameObjectTypeUtils final
    {
    private:
        // === Attributes === //
        inline static const std::array<GameObjectType, 12> valid_types = {
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
}