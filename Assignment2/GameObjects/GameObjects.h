#pragma once

#include "../common/ActionRequest.h"

#include "../Config/ConfigReader.h"

#include "Direction.h"
#include "GameObjectType.h"

#include "../Utils/TankCooldownHandler.h"
#include "../GameBoard/BoardCell.h"
#include "../GameDrawer/DrawableObject.h"
#include "../GameDrawer/DrawingTypes.h"

#include <string>
#include <optional>

// forward declaration
class GameBoard;

// ===========================
// Base Class
// ===========================
class GameObject : public DrawableObject
{
protected:
    // === Attributes === //
    int hp;
    const int collision_damage;
    GameBoard &board;

public:
    // === Constructors === //
    GameObject(GameBoard &b, int hp, int collision_damage);
    virtual ~GameObject() override = default;

    // === Type & Drawing === //
    virtual GameObjectType getObjectType() const = 0;
    virtual std::string getDrawing(DrawingType t) const override = 0;

    // === HP Management === //
    int getHP() const;
    void setHP(int new_hp);
    void gotHit(int dmg = 1);

    // === Damage Management === //
    int getCollisionDamage() const;
};

// ===========================
// StaticObject (no movement)
// ===========================
class StaticObject : public GameObject
{
public:
    StaticObject(GameBoard &b, int hp, int collision_damage);
    virtual ~StaticObject() override = default;
};

// ===========================
// MovableObject (with speed & direction)
// ===========================
class MovableObject : public GameObject
{
protected:
    // === Attributes === //
    Direction direction;
    int speed;

    // === Protected Movement Properties === //
    void setDirection(Direction new_dir);
    void setSpeed(int new_speed);

public:
    MovableObject(GameBoard &b, Direction dir, int spd, int hp, int collision_damage);
    virtual ~MovableObject() override = default;

    // === Movement Properties === //
    Direction getDirection() const;
    int getSpeed() const;
};

// ===========================
// Static Classes
// ===========================
class Mine : public StaticObject
{
public:
    Mine(GameBoard &b);
    ~Mine() override = default;

    // === Type & Drawing === //
    GameObjectType getObjectType() const override;
    std::string getDrawing(DrawingType t) const override;
};

class Wall : public StaticObject
{
public:
    explicit Wall(GameBoard &b);
    ~Wall() override = default;

    // === Type & Drawing === //
    GameObjectType getObjectType() const override;
    std::string getDrawing(DrawingType t) const override;
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject
{
private:
    // === Attributes === //
    const GameObjectType type;
    int shells;

    CooldownHandler cooldown_handler;

    // === Action Validation & Execution === //
    bool validateAndPerformAction(ActionRequest action);
    bool performBackwardAction();
    void performPostponedBackward(bool was_due_before_action);
    bool performForwardAction();
    bool performTurnAction(ActionRequest command);
    bool performShootAction();
    bool performBattleInfoAction();

    // === Movement Support === //
    bool canMoveToCell(BoardCell target) const;
    std::optional<BoardCell> getForwardCell() const;
    std::optional<BoardCell> getBackwardCell() const;
    std::optional<BoardCell> getCurrentCell() const;
    void moveToCell(BoardCell c);
    bool canMoveBackwards() const;
    void moveBackwards();
    void turn(ActionRequest command);

    // === Shooting Support === //
    void shoot();
    void setShells(int new_shells);
    void reload(int amount); // currently not in use

public:
    Tank(GameBoard &b, GameObjectType t, Direction dir, size_t tank_num_shells);

    // === Desturctor=== //
    ~Tank() override = default;

    // === Type & Drawing === //
    GameObjectType getObjectType() const override;
    std::string getDrawing(DrawingType t) const override;

    // === Tank-Specific Actions === //
    bool playTankRound(ActionRequest command);

    // === Shells Management === //
    int getShells() const;
    bool canShoot() const;
};

class Shell : public MovableObject
{
public:
    Shell(GameBoard &b, Direction dir);

    // === Desturctor=== //
    ~Shell() override = default;

    // === Type & Drawing === //
    GameObjectType getObjectType() const override;
    std::string getDrawing(DrawingType t) const override;

    // === Behavior === //
    void advance();
};