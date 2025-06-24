#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"
namespace UserCommon_211388913_322330820
{

    // === Constructors === //
    MovableObject::MovableObject(GameBoard &b, Direction dir, int spd, int hp, int collision_damage)
        : GameObject(b, hp, collision_damage), direction(dir), speed(spd) {}

    // === Getters === //
    Direction MovableObject::getDirection() const
    {
        return direction;
    }

    int MovableObject::getSpeed() const
    {
        return speed;
    }

    // === Setters === //
    void MovableObject::setDirection(Direction new_dir)
    {
        direction = new_dir;
    }

    void MovableObject::setSpeed(int new_speed)
    {
        speed = new_speed;
    }
}