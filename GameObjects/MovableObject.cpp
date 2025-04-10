#include "GameObjects.h"

MovableObject::MovableObject(Direction dir, int spd)
    : GameObject(), direction(dir), speed(spd) {}

MovableObject::MovableObject(Direction dir, int spd, int hp)
    : GameObject(hp), direction(dir), speed(spd) {}


Direction MovableObject::getDirection() const {
    return direction;
}

int MovableObject::getSpeed() const {
    return speed;
}


void MovableObject::setDirection(Direction new_dir) {
    direction = new_dir;
}

void MovableObject::setSpeed(int new_speed) {
    speed = new_speed;
}
