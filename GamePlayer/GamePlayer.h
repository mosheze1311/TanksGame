#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../Action.h"

using namespace std;

class Player{
public:
    Player();
    vector<Action> getActions(); // this function should get params that gives relevant board data to the player so he is able to strategize
};
