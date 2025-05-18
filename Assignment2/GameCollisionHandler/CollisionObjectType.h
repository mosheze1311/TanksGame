#pragma once
#include "../GameObjects/GameObjectType.h" 


enum class CollisionObjectType {
    TANK,
    WALL,
    MINE,
    SHELL
};

namespace CollisionObjectTypeUtils {
    inline CollisionObjectType fromGameObjectType(GameObjectType t) {
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