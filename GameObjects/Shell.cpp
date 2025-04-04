#include "GameObjects.h"


Shell::Shell(Direction dir, int spd)
    : MovableObject(game_object::shell, dir, spd, 1) {}  

void Shell::printType() const {
    std::cout << "Shell (HP: " << getHP() << ")" << std::endl;
}

void Shell::move() {
    std::cout << "Shell moves " << getSpeed()
              << " units in direction " << static_cast<int>(getDirection()) << std::endl;
}

void Shell::destroyed() {
    setHP(0);
    std::cout << "Shell destroyed!" << std::endl;
}