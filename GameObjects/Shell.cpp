#include "GameObjects.h"


Shell::Shell(Direction dir, int spd)
    : MovableObject(dir, spd, 1) {}  

void Shell::printType() const{
}
GameObjectType Shell::getObjectType() const { return GameObjectType::shell; }

void Shell::move() {

}

void Shell::destroyed() {
    setHP(0);
}