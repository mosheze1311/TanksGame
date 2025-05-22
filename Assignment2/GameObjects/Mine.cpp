#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

//=== Constructor ===
Mine::Mine(GameBoard &b, int hp)
    : StaticObject(b, hp) {}

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