#include "GameObjects.h"


// Constructor implementations
GameObject::GameObject() : GameObject(1) {}
GameObject::GameObject(int hp) : hp(hp) {}

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