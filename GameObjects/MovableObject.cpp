#include "GameObjects.h"

MovableObject::MovableObject(GameBoard& b, Direction dir, int spd)
    : GameObject(b), direction(dir), speed(spd) {}

MovableObject::MovableObject(GameBoard& b, Direction dir, int spd, int hp)
    : GameObject(b, hp), direction(dir), speed(spd) {}


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

virtual void move()
