#include "GameObjects.h"


// Constructor implementations
GameObject::GameObject(game_object t) : type(t), hp(1) {}
GameObject::GameObject(game_object t, int hp) : type(t), hp(hp) {}

// Virtual destructor
GameObject::~GameObject() {}

// Virtual function implementation (empty unless overridden)
void GameObject::destroyed() {}

// Getter implementations
game_object GameObject::getType() const
{
    return type;
}

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