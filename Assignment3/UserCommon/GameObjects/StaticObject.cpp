#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

// === Constructor === //
StaticObject::StaticObject(GameBoard &b, int hp, int collision_damage) : GameObject(b, hp, collision_damage) {}