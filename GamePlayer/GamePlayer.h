#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
using namespace std;

enum class Action{
    FORWARD,
    BACKWARD,
    TURN45,
    TURN90,
    FIRE
};

class Player{
public:
    Player();
    vector<Action> getActions(); // this function should get params that gives relevant board data to the player so he is able to strategize
};