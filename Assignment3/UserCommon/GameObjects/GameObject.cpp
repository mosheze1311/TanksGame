#include "GameObjects.h"

#include "../GameBoard/GameBoard.h"
namespace UserCommon_211388913_322330820
{

    // Constructor implementations
    GameObject::GameObject(GameBoard &b, int hp, int collision_damage) : hp(hp), collision_damage(collision_damage), board(b) {}

    int GameObject::getHP() const
    {
        return hp;
    }

    // Setter implementation
    void GameObject::setHP(int new_hp)
    {
        hp = new_hp;
    }

    // Handling getting hit
    void GameObject::gotHit(int dmg)
    {
        this->setHP(getHP() - dmg);
        if (this->getHP() <= 0)
        {
            this->board.removeObject(this);
        }
    }

    int GameObject::getCollisionDamage() const
    {
        return this->collision_damage;
    }
}