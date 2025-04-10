#include "GameObjects.h"

Tank::Tank(GameBoard& b, GameObjectType t, Direction dir, int spd=1, int hp=1)
    : MovableObject(b, dir, spd, hp), type(t) {}

void Tank::printType() const {
}
GameObjectType Tank::getObjectType() const {return type;}

// void Tank::move() {
// }
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

void Tank::action(TankAction command) {

    static int turn_count = 0;
    turn_count++;

    switch (command) {
        case TankAction::NOTHING:
            
            break;
        case TankAction::FORWARD:
            
            break;
        case TankAction::BACKWARD:
           
            break;
        case TankAction::TURN45LEFT:
            
            break;
        case TankAction::TURN45RIGHT:
            
            break;
        case TankAction::TURN90LEFT:
            
            break;
        case TankAction::TURN90RIGHT:
            
            break;
        case TankAction::FIRE:
            
            break;
        default:
            
            break;
    }
}