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
    int hp;
    GameBoard &board;

public:
    explicit GameObject(GameBoard &b);
    GameObject(GameBoard &b, int hp);
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
    Direction direction;
    int speed;

public:
    MovableObject(GameBoard &b, Direction dir, int spd);
    MovableObject(GameBoard &b, Direction dir, int spd, int hp);

    Direction getDirection() const;
    int getSpeed() const;

    void setDirection(Direction new_dir);
    void setSpeed(int new_speed);
};

// ===========================
// Static Classes
// ===========================
class Mine : public StaticObject
{
public:
    explicit Mine(GameBoard &b);

    void printType() const override;
    GameObjectType getObjectType() const override;
    void destroyed() override;
    string getDrawing(DrawingType t) const override;
};

class Wall : public StaticObject
{
public:
    explicit Wall(GameBoard &b);
    void printType() const override;
    GameObjectType getObjectType() const override;

    void destroyed() override;
    string getDrawing(DrawingType t) const override;
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject
{
private:
    const GameObjectType type;
    int shells = 16;
    int turns_to_wait_for_backward = -2;
    int shoot_cooldown = 0;

    //===Functions===
    void tickShootCooldown();
    bool canMoveToCell(BoardCell target);
    bool validateAndPerformAction(TankAction action);
    optional<BoardCell> getForwardCell();
    optional<BoardCell> getBackwardCell();
    optional<BoardCell> getCurrentCell();
    
    //===Backwards-Wait Handling Functions===
    void tickBackwardsWait(); // decreases wait counter if needed
    bool isPendingBackwards() const;
    bool notPendingBackwards() const;
    bool canImmediateBackwards() const;

    void extendBackwardsStreak();
    void cancelBackwardsWait();
    void startBackwardsWait();

    //===Perform Actions Functions===
    bool performBackwardAction();
    bool performForwardAction();
    bool performTurnAction(TankAction command);
    bool performShootAction();

    void moveToCell(BoardCell c);
    void shoot();

    bool canMoveBackwards();
    void moveBackwards();
    
    void turn(TankAction command);

public:
    Tank(GameBoard &b,
         GameObjectType t = GameObjectType::tank1,
         Direction dir = Direction::UP,
         int spd = 1,
         int hp = 1);

    bool canShoot() const; // returns true if not waiting
    void printType() const override;
    GameObjectType getObjectType() const override;
    bool action(TankAction command);
    void destroyed() override;

    int getShells() const;
    void setShells(int new_shells);
    void reload(int amount);
    int getShootCooldown() const;
    int getBackwardWait() const;

    //===Drawable Interface===
    string getDrawing(DrawingType t) const override;
};

class Shell : public MovableObject
{
public:
    Shell(GameBoard &b, Direction dir, int spd = 2);

    void printType() const override;
    GameObjectType getObjectType() const override;
    void action();
    void destroyed() override;
    string getDrawing(DrawingType t) const override;
};
