#include "GameObjects.h"


Shell::Shell(GameBoard& b, Direction dir, int spd)
    : MovableObject(b, dir, spd, 1) {}

void Shell::printType() const{
}
GameObjectType Shell::getObjectType() const { return GameObjectType::shell; }

void Shell::destroyed() {
    setHP(0);
}

void Shell::action(ShellAction command, Direction dir){
    
    switch (dir)
    {
    case Direction::UP:
        /* code */
        break;
    
    case Direction::UPR:
        /* code */
        break;
    case Direction::UPL:
        /* code */
        break;
    case Direction::RIGHT:
        /* code */
        break;
    case Direction::LEFT:
        /* code */
        break;
    case Direction::DOWN:
        /* code */
        break;
    case Direction::DOWNR:
        /* code */
        break;
    case Direction::DOWNL:
        /* code */
        break;

    default:
        break;
    }
}
