#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

Wall::Wall(GameBoard& b)
    : StaticObject(b, 2) {}

void Wall::printType() const {
}
GameObjectType Wall::getObjectType() const { return GameObjectType::wall; }

void Wall::destroyed() {
    setHP(0);
}

string Wall::getDrawing(DrawingType t) const
{
    switch (t)
    {
    case DrawingType::REGULAR:
        return this->getHP() == 2 ? "⬛" : "🟫";
    case DrawingType::TENNIS:
        return this->getHP() == 2 ? "🥅" : "🥅";
    case DrawingType::SCIFI:
        return this->getHP() == 2 ? "📡" : "🛰️\0";
    case DrawingType::PIRATE:
        return this->getHP() == 2 ? "🌊" : "🪸\0";
    case DrawingType::MIDDLE_EAST:
        return this->getHP() == 2 ? "🏰" : "🛡️\0";
    default:
        return this->getHP() == 2 ? "⬛" : "🟫";
    }
}
