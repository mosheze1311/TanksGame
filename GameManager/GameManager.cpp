# include "GameManager.h"
#include "../GameDrawer.cpp"
#include <thread>
#include <chrono>

GameManager::GameManager(GameBoard board, Player p1, Player p2, string output_file): board(board) {};
bool GameManager::isUserActionValid(pair<Tank *, Action> action_pair) const {return true;};
 void GameManager::performPlayerActionsOnBoard(vector<pair<Tank *, Action>> actions)
{
    for (pair<Tank*, Action> action_pair : actions)
    {
        // skip invalid actions
        if (! this->isUserActionValid(action_pair)){
            // TODO: log as invalid action.
            continue;
        }

        // TODO: perform the actions.
    }
}

void GameManager::play(){
    GameDrawer d(board);
    while (true)
    {
        vector<Action> t1_actions = p1.getActions();
        vector<Action> t2_actions = p2.getActions();
        board.moveTanksRandomly();
        d.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
};