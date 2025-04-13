#include "GameObjects.h"

#include "../GameBoard/GameBoard.h"

// Constructor implementations
GameObject::GameObject(GameBoard& b) : hp(1), board(b) {}
GameObject::GameObject(GameBoard& b, int hp) : hp(hp), board(b)  {}

// Virtual destructor
GameObject::~GameObject() {}

// Virtual function implementation (empty unless overridden)
void GameObject::destroyed() {}

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
    hp -= dmg;
    if (this->getHP() <= 0)
    {
        this->board.removeObject(this);
    }
}