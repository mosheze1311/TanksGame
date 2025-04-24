#pragma once

#include "TankAction.h"
#include "Direction.h"
#include "GameObjectType.h"
#include "../GameDrawer/DrawingTypes.h"
#include <string>
#include <optional>

using namespace std;

// forward declaration
class GameBoard;
class BoardCell;

// ===========================
// Base Class
// ===========================
class DrawableObject
{
public:
    virtual string getDrawing(DrawingType t) const = 0;
};

class GameObject : public DrawableObject
{
protected:
    //=== Attributes ===
    int hp;
    GameBoard &board;

public:
    explicit GameObject(GameBoard &b);
    GameObject(GameBoard &b, int hp);
    virtual ~GameObject();

    //=== Type & Drawing ===
    virtual GameObjectType getObjectType() const = 0;
    string getDrawing(DrawingType t) const override = 0;

    //=== HP Management ===
    int getHP() const;
    void setHP(int new_hp);
    void gotHit(int dmg = 1);
};

// ===========================
// StaticObject (no movement)
// ===========================
class StaticObject : public GameObject
{
public:
    StaticObject(GameBoard &b, int hp);
};

// ===========================
// MovableObject (with speed & direction)
// ===========================
class MovableObject : public GameObject
{
protected:
    //=== Attributes ===
    Direction direction;
    int speed;

    //=== Protected Movement Properties ===
    void setDirection(Direction new_dir);
    void setSpeed(int new_speed);

public:
    MovableObject(GameBoard &b, Direction dir, int spd);
    MovableObject(GameBoard &b, Direction dir, int spd, int hp);

    //=== Movement Properties ===
    Direction getDirection() const;
    int getSpeed() const;
};

// ===========================
// Static Classes
// ===========================
class Mine : public StaticObject
{
public:
    explicit Mine(GameBoard &b);

    //=== Type & Drawing ===
    GameObjectType getObjectType() const override;
    string getDrawing(DrawingType t) const override;
};

class Wall : public StaticObject
{
public:
    explicit Wall(GameBoard &b);

    //=== Type & Drawing ===
    GameObjectType getObjectType() const override;
    string getDrawing(DrawingType t) const override;
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject
{
private:
    //=== Attributes ===
    const GameObjectType type;
    int shells = 16;
    int turns_to_wait_for_backward = -2;
    int shoot_cooldown = 0;

    //=== Cooldown / Wait Management ===
    void tickShootCooldown();
    void tickBackwardsWait();
    bool isPendingBackwards() const;
    bool canImmediateBackwards() const;
    void extendBackwardsStreak();
    void cancelBackwardsWait();
    void startBackwardsWait();

    //=== Action Validation & Execution ===
    bool validateAndPerformAction(TankAction action);
    bool performBackwardAction();
    bool performForwardAction();
    bool performTurnAction(TankAction command);
    bool performShootAction();

    //=== Movement Support ===
    bool canMoveToCell(BoardCell target) const;
    optional<BoardCell> getForwardCell() const;
    optional<BoardCell> getBackwardCell() const;
    optional<BoardCell> getCurrentCell() const;
    void moveToCell(BoardCell c);
    bool canMoveBackwards() const;
    void moveBackwards();
    void turn(TankAction command);

    //=== Shooting Support ===
    void shoot();
    void setShells(int new_shells);
    void reload(int amount); // currently not in use
    int getShootCooldown() const;
    int getBackwardWait() const;

public:
    Tank(GameBoard &b,
         GameObjectType t = GameObjectType::tank1,
         Direction dir = Direction::UP,
         int spd = 1,
         int hp = 1);

    //=== Type & Drawing ===
    GameObjectType getObjectType() const override;
    string getDrawing(DrawingType t) const override;

    //=== Tank-Specific Actions ===
    bool action(TankAction command);

    //=== Shells Management ===
    int getShells() const;
    bool canShoot() const;
};

class Shell : public MovableObject
{
public:
    Shell(GameBoard &b, Direction dir, int spd = 2);

    //=== Type & Drawing ===
    GameObjectType getObjectType() const override;
    string getDrawing(DrawingType t) const override;

    //=== Behavior ===
    void advance();
};