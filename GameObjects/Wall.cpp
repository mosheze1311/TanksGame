#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

//=== Constructors ===
Wall::Wall(GameBoard& b)
    : StaticObject(b, 2) {}

//=== Type Info ===
GameObjectType Wall::getObjectType() const { return GameObjectType::WALL; }

//=== Drawing ===
string Wall::getDrawing(DrawingType t) const
{
    switch (t)
    {
    case DrawingType::REGULAR:
        return this->getHP() == 2 ? "⬛" : "🟫";
    case DrawingType::TENNIS:
        return this->getHP() == 2 ? "🥅" : "🥅";
    case DrawingType::SCIFI:
        return this->getHP() == 2 ? "📡" : "🛰️";
    case DrawingType::PIRATE:
        return this->getHP() == 2 ? "🌊" : "🪸";
    case DrawingType::MIDDLE_EAST:
        return this->getHP() == 2 ? "🏰" : "🛡️";
    default:
        return this->getHP() == 2 ? "⬛" : "🟫";
    }
}
