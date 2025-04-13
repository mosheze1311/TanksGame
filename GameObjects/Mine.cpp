#include "GameObjects.h"
#include "../GameBoard/GameBoard.h"

Mine::Mine(GameBoard& b)
    : StaticObject(b, 1) {}

void Mine::printType() const {
}

GameObjectType Mine::getObjectType() const{return GameObjectType::mine;}

void Mine::destroyed() {
    setHP(0);    
}

string Mine::getDrawing() const{
    return "💥";
}