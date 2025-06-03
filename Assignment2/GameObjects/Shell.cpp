#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

#define shell_hp 1 // not in config because should not be changed

//=== Constructor ===
Shell::Shell(GameBoard &b, Direction dir)
    : MovableObject(b, dir, ConfigReader::getConfig().getShellsSpeed(), shell_hp) {}

//=== Type Info ===
GameObjectType Shell::getObjectType() const { return GameObjectType::SHELL; }

//=== Movement ===
void Shell::advance()
{
    if(auto opt_cell = board.getObjectLocation(this)){
        BoardCell curr_cell = *opt_cell;
        this->board.moveGameObject(this, curr_cell + this->direction);
    }
}

//=== Drawing ===
std::string Shell::getDrawing(DrawingType t) const
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
