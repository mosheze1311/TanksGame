#pragma once
#include "GameObjects.h"  

class StaticObject : public GameObject {  
protected:
    int hp;

public:
    //  constructor that passes game_object to GameObject
    StaticObject(game_object t) : GameObject(t) {}
    // constructor that passes hp
    StaticObject(game_object t, int hp) : GameObject(t), hp(hp) {}

    
};
