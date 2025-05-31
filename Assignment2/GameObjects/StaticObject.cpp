#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

// === Constructor === //
StaticObject::StaticObject(GameBoard &b, int hp) : GameObject(b, hp) {}