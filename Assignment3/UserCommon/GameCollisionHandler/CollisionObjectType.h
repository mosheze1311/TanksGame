#pragma once

#include "../GameObjects/GameObjectType.h"
namespace UserCommon_211388913_322330820
{

    enum class CollisionObjectType
    {
        TANK,
        WALL,
        MINE,
        SHELL
    };

    namespace CollisionObjectTypeUtils
    {
        inline CollisionObjectType fromGameObjectType(GameObjectType t)
        {
            switch (t)
            {
            case GameObjectType::TANK1:
            case GameObjectType::TANK2:
                return CollisionObjectType::TANK;
            case GameObjectType::WALL:
                return CollisionObjectType::WALL;
            case GameObjectType::MINE:
                return CollisionObjectType::MINE;
            case GameObjectType::SHELL:
                return CollisionObjectType::SHELL;
            default:
                return CollisionObjectType::TANK; // should never get here
            }
        }
    }
}