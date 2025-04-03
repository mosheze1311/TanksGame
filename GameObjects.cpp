#pragma once
#include "GameObjects.h"  


class GameObject {
    protected:
        game_object type;
        int hp;
    
    public:
        GameObject(game_object t) : type(t), hp(1) {}
        GameObject(game_object t, int hp) : type(t), hp(hp) {}
        virtual ~GameObject() {}
    
        virtual void printType() const = 0;
        virtual void destroyed() {}  
    
        game_object getType() const { return type; }
    
        int getHP() const { return hp; }
        void setHP(int new_hp) { hp = new_hp; }
    
        void gotHit(int dmg = 1) { hp -= dmg; }
    };
    
