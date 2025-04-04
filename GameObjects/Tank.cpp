#include "GameObjects.h"

Tank::Tank(game_object t, Direction dir, int spd, int hp)
    : MovableObject(t, dir, spd, hp) {}

void Tank::printType() {
    std::cout << "Tank (" << static_cast<char>(type)
              << ") with " << shells
              << " shells, HP: " << getHP() << std::endl;
}

void Tank::move() {
    std::cout << "Tank moves " << getSpeed()
              << " units in direction " << static_cast<int>(getDirection()) << std::endl;
}

void Tank::shoot() {
    if (shells > 0) {
        --shells;
        std::cout << "Bang!" << std::endl;
    } else {
        std::cout << "Out of shells!" << std::endl;
    }
}

void Tank::destroyed() {
    setHP(0);
    shells = 0;
    std::cout << "Tank " << static_cast<char>(type) << " has been destroyed." << std::endl;
}

int Tank::getShells() const {
    return shells;
}

void Tank::setShells(int new_shells) {
    shells = new_shells;
}

void Tank::reload(int amount) {
    shells += amount;
} 