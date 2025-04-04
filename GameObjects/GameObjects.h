#pragma once
#include <iostream>

// ===========================
// Enums
// ===========================
enum class game_object {
    tank1 = '1',
    tank2 = '2',
    wall = '#',
    mine = '@',
    shell = '*'
};

enum class Direction {
    UP, UPR, RIGHT, DOWNR, DOWN, DOWNL, LEFT, UPL
};

// ===========================
// Base Class
// ===========================
class GameObject {
protected:
    game_object type;
    int hp;

public:
    GameObject(game_object t);
    GameObject(game_object t, int hp);
    virtual ~GameObject();

    virtual void printType() = 0;
    virtual void destroyed();

    game_object getType() const;
    void setHP(int new_hp);
    void gotHit(int dmg = 1);
    int getHP() const;
};

// ===========================
// StaticObject (no movement)
// ===========================
class StaticObject : public GameObject {
public:
    StaticObject(game_object t);
    StaticObject(game_object t, int hp);
};

// ===========================
// MovableObject (with speed & direction)
// ===========================
class MovableObject : public GameObject {
protected:
    Direction direction;
    int speed;

public:
    MovableObject(game_object t, Direction dir, int spd);
    MovableObject(game_object t, Direction dir, int spd, int hp);

    virtual void move() = 0;

    Direction getDirection() const;
    int getSpeed() const;

    void setDirection(Direction new_dir);
    void setSpeed(int new_speed);
};

// ===========================
// Static Classes
// ===========================
class Mine : public StaticObject {
public:
    Mine();
    void printType() override;
    void destroyed() override;
};

class Wall : public StaticObject {
public:
    Wall();
    void printType() override;
    void destroyed() override;
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject {
private:
    int shells = 10;

public:
    Tank(game_object t = game_object::tank1,
         Direction dir = Direction::UP,
         int spd = 1,
         int hp = 1);

    void printType() override;
    void move() override;
    void shoot();
    void destroyed() override;

    int getShells() const;
    void setShells(int new_shells);
    void reload(int amount);
};

class Shell : public MovableObject {
public:
    Shell(Direction dir, int spd = 2);

    void printType() override;
    void move() override;
    void destroyed() override;
};
