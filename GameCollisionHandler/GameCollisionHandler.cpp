#include "GameCollisionHandler.h"

std::unordered_set<GameObjectType> GameCollisionHandler::getExplosionList(GameObjectType objType)
{

    if (this->explosion_map.find(objType) != explosion_map.end()){
        return explosion_map[objType];
    }

    return {};
}