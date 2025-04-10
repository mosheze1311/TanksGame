#include "GameObjects.h"

Wall::Wall(GameBoard& b)
    : StaticObject(b, 2) {}

void Wall::printType() const {
}
GameObjectType Wall::getObjectType() const { return GameObjectType::wall; }

void Wall::destroyed() {
    setHP(0);
}