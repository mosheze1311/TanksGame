#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/TankAction.h"

using namespace std;

class Player{
public:
    Player();
    vector<TankAction> getActions(); // this function should get params that gives relevant board data to the player so he is able to strategize
};
