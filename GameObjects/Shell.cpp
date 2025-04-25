#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

//=== Constructor ===
Shell::Shell(GameBoard &b, Direction dir, int spd)
    : MovableObject(b, dir, spd, 1) {}

//=== Type Info ===
GameObjectType Shell::getObjectType() const { return GameObjectType::SHELL; }

//=== Movement ===
void Shell::advance()
{

    board = this->board;
    if(auto opt_cell = board.getObjectLocation(this)){
        BoardCell curr_cell = *opt_cell;
        board.moveGameObject(this, curr_cell + this->direction);
    }
}

//=== Drawing ===
string Shell::getDrawing(DrawingType t) const
{
    switch (t)
    {
    case DrawingType::REGULAR:
        return "🟠";
    case DrawingType::TENNIS:
        return "🎾";
    case DrawingType::SCIFI:
        return "🔮";
    case DrawingType::PIRATE:
        return "💣";
    case DrawingType::MIDDLE_EAST:
        return "🚀";
    default:
        return "🟠";
    }
}