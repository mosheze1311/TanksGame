#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

Shell::Shell(GameBoard &b, Direction dir, int spd)
    : MovableObject(b, dir, spd, 1) {}

void Shell::printType() const
{
}
GameObjectType Shell::getObjectType() const { return GameObjectType::shell; }

void Shell::destroyed()
{
    setHP(0);
}

void Shell::action()
{

    board = this->board;
    if(auto opt_cell = board.getObjectLocation(this)){
        BoardCell curr_cell = *opt_cell;
        board.moveGameObject(this, curr_cell + this->direction);
    }
}
