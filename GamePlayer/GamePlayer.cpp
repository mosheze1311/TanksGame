#include "GamePlayer.h"
#include <queue>


Player::Player(std::vector<Tank*>& tanks) : player_tanks(player_tanks) {}

vector<TankAction> Player::getActionsFromFirstPlayer(GameBoard &board){

    vector<TankAction> res;

    for (Tank* tank:player_tanks){
        // Find bfs/dfs path for each tank. This is expensive, need to check
        auto nearestCell = BFS(board, board.getObjectLocation(tank), GameObjectType::tank2);

        if (nearestCell == NULL){
            // should never happen- if NULL, than game should be over.
            res.push_back(TankAction::NOTHING); 
            continue;
        }


        // Needs to complete logic- adjust direction and move towards the taget.
        





    }





};


vector<TankAction> Player::getActionsFromSecondPlayer(GameBoard &board){





}