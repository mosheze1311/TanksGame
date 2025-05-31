#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

#define mine_hp 1 // not in config because should not be changed

//=== Constructor ===
Mine::Mine(GameBoard &b)
    : StaticObject(b, mine_hp) {}

//=== Type Information ===
GameObjectType Mine::getObjectType() const
{
    return GameObjectType::MINE;
}

//=== Drawing ===
std::string Mine::getDrawing(DrawingType t) const
{
    switch (t)
    {
    case DrawingType::REGULAR:
        return "💥";
    case DrawingType::TENNIS:
        return "🔥";
    case DrawingType::SCIFI:
        return "💥";
    case DrawingType::PIRATE:
        return "☠️ ";
    case DrawingType::MIDDLE_EAST:
        return "🧨";
    default:
        return "💥"; // fallback symbol
    }
}