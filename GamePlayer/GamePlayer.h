#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/TankAction.h"

using namespace std;

class Player{

private:
std::vector<Tank*> player_tanks;

public:
    Player(std::vector<Tank*>& tanks) : player_tanks(tanks) {}
    vector<TankAction> getActionsFromFirstPlayer(GameBoard &board); // Strategy for first player

    vector<TankAction> getActionsFromSecondPlayer(GameBoard &board);// Strategy for second player
    
    BoardCell BFS(GameBoard& board, BoardCell startingCell, GameObjectType target); // Sholud have loguc for BFS search
    
    
};
