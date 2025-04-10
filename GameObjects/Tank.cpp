#include "GameObjects.h"

Tank::Tank(GameObjectType t, Direction dir, int spd, int hp)
    : MovableObject(dir, spd, hp), type(t){}

void Tank::printType() const {
}
GameObjectType Tank::getObjectType() const {return type;}

void Tank::move() {
}
void Tank::shoot() {
}
void Tank::destroyed() {
    setHP(0);
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