#include "GameObjects.h"

Wall::Wall() : StaticObject(game_object::wall, 2) {}

void Wall::printType() {
    std::cout << "Wall (HP: " << getHP() << ")" << std::endl;
}

void Wall::destroyed() {
    setHP(0);
    std::cout << "Wall has been destroyed." << std::endl;
}