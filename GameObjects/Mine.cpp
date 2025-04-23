#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

Mine::Mine(GameBoard& b)
    : StaticObject(b, 1) {}

void Mine::printType() const {
}

GameObjectType Mine::getObjectType() const{return GameObjectType::mine;}

void Mine::destroyed() {
    setHP(0);    
}

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