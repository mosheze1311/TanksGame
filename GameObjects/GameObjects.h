#pragma once
#include "../GameBoard/GameBoard.h"
#include "../Action.h"
// ===========================
// Enums
// ===========================
enum class GameObjectType
{
    tank1 = '1',
    tank2 = '2',
    wall = '#',
    mine = '@',
    shell = '*'
};

enum class Direction
{
    UP = 0,
    UPR = 1,
    RIGHT = 2,
    DOWNR = 3,
    DOWN = 4,
    DOWNL = 5,
    LEFT = 6,
    UPL = 7
};

constexpr std::pair<int, int> offset(Direction dir)
{
    switch (dir)
    {
    case Direction::UP:
        return {0, 1};
    case Direction::UPR:
        return {1, 1};
    case Direction::RIGHT:
        return {1, 0};
    case Direction::DOWNR:
        return {1, -1};
    case Direction::DOWN:
        return {0, -1};
    case Direction::DOWNL:
        return {-1, -1};
    case Direction::LEFT:
        return {-1, 0};
    case Direction::UPL:
        return {-1, 1};
    }
    return {0, 0}; // should never hit this
}

// ===========================
// Base Class
// ===========================
class GameObject {
protected:
    int hp;
    GameBoard& board;

public:
    GameObject(GameBoard& b);
    GameObject(GameBoard& b, int hp);
    virtual ~GameObject();

    virtual void printType() const = 0;
    virtual void destroyed();

    virtual GameObjectType getObjectType() const = 0;
    void setHP(int new_hp);
    void gotHit(int dmg = 1);
    int getHP() const;
};

// ===========================
// StaticObject (no movement)
// ===========================
class StaticObject : public GameObject {
public:
    StaticObject(GameBoard& b, int hp);
};

// ===========================
// MovableObject (with speed & direction)
// ===========================
class MovableObject : public GameObject {
protected:
    Direction direction;
    int speed;

public:
    MovableObject(GameBoard& b, Direction dir, int spd);
    MovableObject(GameBoard& b, Direction dir, int spd, int hp);

    virtual void action();

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
    Mine(GameBoard& b);
    
    void printType() const override;
    GameObjectType getObjectType() const override;
    void destroyed() override;
};

class Wall : public StaticObject {
public:
    Wall(GameBoard& b);
    void printType() const override;
    GameObjectType getObjectType() const override;

    void destroyed() override;
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject {
private:
    const GameObjectType type;
    int shells = 16;

public:
    Tank(GameBoard& b,
        GameObjectType t = GameObjectType::tank1,
        Direction dir = Direction::UP,
        int spd = 1,
        int hp = 1);

    void printType() const override;
    GameObjectType getObjectType() const override;
    void action(TankAction command);
    void shoot();
    void destroyed() override;

    int getShells() const;
    void setShells(int new_shells);
    void reload(int amount);
};

class Shell : public MovableObject {
public:
    Shell(GameBoard& b, Direction dir, int spd = 2);

    void printType() const override;
    GameObjectType getObjectType() const override;
    void action();
    void destroyed() override;
};
