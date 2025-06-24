#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"
namespace UserCommon_211388913_322330820
{

    // === Constructor === //
    StaticObject::StaticObject(GameBoard &b, int hp, int collision_damage) : GameObject(b, hp, collision_damage) {}
}