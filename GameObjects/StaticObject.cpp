#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

StaticObject::StaticObject(GameBoard& b, int hp)
    : GameObject(b, hp) {}
