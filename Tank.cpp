#pragma once
#include "GameObjects.h"

class Tank : public MovableObject {
    private:
        int shells = 10;
        
    
    public:
        // Constructor with default values
        Tank(game_object t = game_object::tank1,
             Direction dir = Direction::UP,
             int spd = 1,
             int hp = 1)
            : MovableObject(t, dir, spd, hp) {}
    
        void printType() const override {
            std::cout << "Tank (" << static_cast<char>(type)
                      << ") with " << shells << " shells, HP: " << getHp() << std::endl;
        }
    
        void move() override {
            std::cout << "Tank moves " << getSpeed()
                      << " units in direction " << static_cast<int>(getDirection()) << std::endl;
        }
    
        void shoot() {
            if (shells > 0) {
                --shells;
                std::cout << "Bang!" << std::endl;
            } else {
                std::cout << "Out of shells!" << std::endl;
            }
        }


        void destroyed(){

        }
    
        int getShells() const { return shells; }
        void setShells(int new_shells){shells = new_shells;}
        void reload(int amount) { shells += amount; }
    };
    