#include "GameObjects.h"

Mine::Mine()
    : StaticObject(1) {}

void Mine::printType() const {
}

GameObjectType Mine::getObjectType() const{return GameObjectType::mine;}

void Mine::destroyed() {
    setHP(0);    
}

