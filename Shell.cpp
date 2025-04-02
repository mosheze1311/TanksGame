#pragma once
#include "GameObjects.h"


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
    