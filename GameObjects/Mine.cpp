#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

//=== Constructor ===
Mine::Mine(GameBoard &b)
    : StaticObject(b, 1) {}

//=== Type Information ===
GameObjectType Mine::getObjectType() const
{
    return GameObjectType::mine;
}

//=== Drawing ===
string Mine::getDrawing(DrawingType t) const
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