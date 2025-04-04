#include "GameObjects.h"

Mine::Mine()
    : StaticObject(game_object::mine, 1) {}

void Mine::printType() {
    std::cout << "Mine (HP: " << getHP() << ")" << std::endl;
}

void Mine::destroyed() {
    setHP(0);
    std::cout << "Mine exploded!" << std::endl;
    
}

