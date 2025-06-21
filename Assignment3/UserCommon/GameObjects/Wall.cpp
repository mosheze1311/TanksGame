#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

#define wall_collision_damage 1 // not in config because should not be changed

//=== Constructors ===
Wall::Wall(GameBoard &b)
    : StaticObject(b, ConfigReader::getConfig().getWallHp(), wall_collision_damage) {}

//=== Type Info ===
GameObjectType Wall::getObjectType() const { return GameObjectType::WALL; }

//=== Drawing ===
std::string Wall::getDrawing(DrawingType t) const
{
    switch (t)
    {
    case DrawingType::REGULAR:
        return this->getHP() != 1 ? "⬛" : "🟫";
    case DrawingType::TENNIS:
        return this->getHP() != 1 ? "🥅" : "🥅";
    case DrawingType::SCIFI:
        return this->getHP() != 1 ? "📡" : "🛰️ ";
    case DrawingType::PIRATE:
        return this->getHP() != 1 ? "🌊" : "🪸 ";
    case DrawingType::MIDDLE_EAST:
        return this->getHP() != 1 ? "🏰" : "🛡️ ";
    default:
        return this->getHP() != 1 ? "⬛" : "🟫";
    }
}