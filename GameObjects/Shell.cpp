#include "GameObjects.h"


Shell::Shell(GameBoard& b, Direction dir, int spd)
    : MovableObject(b, dir, spd, 1) {}

void Shell::printType() const{
}
GameObjectType Shell::getObjectType() const { return GameObjectType::shell; }

void Shell::destroyed() {
    setHP(0);
}

void Shell::action(){
    
    board = this->board;
    BoardCell curr_cell = board.getobjectLocation(this);
    board.moveGameObject(this, curr_cell + this->direction);
}
