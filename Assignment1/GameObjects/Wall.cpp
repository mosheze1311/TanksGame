#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

//=== Constructors ===
Wall::Wall(GameBoard& b, int hp)
    : StaticObject(b, hp) {}

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

//=== Copy ===
GameObject *Wall::copy(GameBoard &copy_new_board) const
{
    return new Wall(copy_new_board, this->hp);
}