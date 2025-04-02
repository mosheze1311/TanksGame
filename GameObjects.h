#pragma once
#include <iostream>

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
    GameObject(game_object t) : type(t), hp(1) {}
    GameObject(game_object t, int hp) : type(t), hp(hp) {}
    virtual ~GameObject() {}

    virtual void printType() const = 0;

    game_object getType() const { return type; }

    void setHP(int new_hp) { hp = new_hp; }
    void gotHit(int dmg = 1) { hp -= dmg; }
    int getHp() const { return hp; }
};

// ===========================
// StaticObject (no movement)
// ===========================
class StaticObject : public GameObject {
public:
    StaticObject(game_object t) : GameObject(t) {}
    StaticObject(game_object t, int hp) : GameObject(t, hp) {}
};

// ===========================
// MovableObject (has speed & direction)
// ===========================
class MovableObject : public GameObject {
protected:
    Direction direction;
    int speed;

public:
    MovableObject(game_object t, Direction dir, int spd)
        : GameObject(t), direction(dir), speed(spd) {}

    MovableObject(game_object t, Direction dir, int spd, int hp)
        : GameObject(t, hp), direction(dir), speed(spd) {}

    virtual void move() = 0;

    Direction getDirection() const { return direction; }
    int getSpeed() const { return speed; }

    void setDirection(Direction new_dir) { direction = new_dir; }
    void setSpeed(int new_speed) { speed = new_speed; }
};

// ===========================
// Static Classes
// ===========================
class Mine : public StaticObject {
public:
    Mine() : StaticObject(game_object::mine) {}

    void printType() const override {
        std::cout << "Mine" << std::endl;
    }
};

class Wall : public StaticObject {
public:
    Wall() : StaticObject(game_object::wall, 2) {}

    void printType() const override {
        std::cout << "Wall (HP: " << getHp() << ")" << std::endl;
    }
};

// ===========================
// Movable Classes
// ===========================
class Tank : public MovableObject {
private:
    int shells = 10;

public:
    Tank(game_object t = game_object::tank1, Direction dir = Direction::UP, int spd = 1, int hp = 1)
        : MovableObject(t, dir, spd, hp) {}

    void printType() const override {
        std::cout << "Tank (" << static_cast<char>(type) << ") with " << shells
                  << " shells, HP: " << getHp() << std::endl;
    }

    void move() override {
        std::cout << "Tank moves " << getSpeed() << " units in direction "
                  << static_cast<int>(getDirection()) << std::endl;
    }

    void shoot() {
        if (shells > 0) {
            --shells;
            std::cout << "Bang!" << std::endl;
        }
    }
};

class Shell : public MovableObject {
public:
    Shell(Direction dir, int spd = 2)
        : MovableObject(game_object::shell, dir, spd) {}

    void printType() const override {
        std::cout << "Shell" << std::endl;
    }

    void move() override {
        std::cout << "Shell moves " << getSpeed() << " units in direction "
                  << static_cast<int>(getDirection()) << std::endl;
    }
};
