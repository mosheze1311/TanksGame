#include "GameObjects.h"


// Constructor implementations
GameObject::GameObject(GameBoard& b) : board(b), hp(1) {}
GameObject::GameObject(GameBoard& b, int hp) : board(b), hp(hp) {}

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
}