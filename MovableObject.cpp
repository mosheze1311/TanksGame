#pragma once
#include "GameObjects.h"

class MovableObject : public GameObject {
    protected:
        Direction direction;
        int speed;
        int hp;

    public:
        MovableObject(game_object t, Direction dir, int spd)
            : GameObject(t), direction(dir), speed(spd) {}
        
        MovableObject(game_object t, Direction dir, int spd, int hp)
            : GameObject(t), direction(dir), speed(spd), hp(hp) {}
    
        virtual void move() = 0;
    
        Direction getDirection() const { return direction; }
        int getSpeed() const { return speed; }
        int getHP() const { return hp; }
    
        void setDirection(Direction new_dir) { direction = new_dir; }
        void setSpeed(int new_speed) { speed = new_speed; }
        
    };
    