#pragma once
#include "GameObjects.h"

class MovableObject : public GameObject {
protected:
    Direction direction;
    int speed;

public:
    MovableObject(game_object t, Direction dir, int spd)
        : GameObject(t), direction(dir), speed(spd) {}

    MovableObject(game_object t, Direction dir, int spd, int hp)
        : GameObject(t, hp), direction(dir), speed(spd) {}

    virtual void move() = 0;

    Direction getDirection() const { return direction; }
    int getSpeed() const { return speed; }

    

    void setDirection(Direction new_dir) { direction = new_dir; }
    void setSpeed(int new_speed) { speed = new_speed; }
};
