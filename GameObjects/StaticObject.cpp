#include "GameObjects.h"  

StaticObject::StaticObject(game_object t)
    : GameObject(t) {}

StaticObject::StaticObject(game_object t, int hp)
    : GameObject(t, hp) {}
