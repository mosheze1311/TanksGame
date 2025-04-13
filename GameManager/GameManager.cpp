# include "GameManager.h"
#include "../GameDrawer/GameDrawer.h"
#include <thread>
#include <chrono>
#include "../Logger/Logger.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

GameManager::GameManager(GameBoard& board, Player p1, Player p2, string output_file): board(board) {};

int GameManager::getRemainingTurns() const{
    return this->remaining_turns;
}

void logWin(bool is_player1_winner) {
    std::string winner = is_player1_winner ? "Player1 is the winner!" : "Player2 is the winner!";
    Logger::runtime().logError("The winner is: " + winner);
}

void GameManager::logTie(){
    Logger::runtime().logError("The game is tied!");
}

void GameManager::logAction(Tank* tank, TankAction action, bool is_valid) {
    std::string status = is_valid ? "Valid" : "Invalid";

    Logger::runtime().logError(
        status + " action: Tank type '" +
        std::string(1, static_cast<char>(tank->getObjectType())) +
        "' tried action number '" + std::to_string(static_cast<int>(action)) + "'"
    );
}


 void GameManager::performPlayerActionsOnBoard(vector<pair<Tank *, TankAction>> actions)
{
    for (pair<Tank*, TankAction> action_pair : actions)
    {
        bool is_valid = action_pair.first->action(action_pair.second);
        logAction(action_pair.first, action_pair.second, is_valid);
    }
}

bool GameManager::concludeGame(){

        if (board.getTotalRemainingShells() <= 0 && this->getRemainingTurns() > 0){
        if (board.getGameObjectCount(GameObjectType::tank1) == 0){
            logWin(true);
            return true;
        }
        else if (board.getGameObjectCount(GameObjectType::tank2) == 0)
        {
            logWin(false);
            return true;
        }
    }
    return false;
}

void GameManager::play(){
    GameDrawer d(this->board);
    d.draw();
    GameCollisionHandler c_handler(this->board);
    while (true)
    {
        vector<TankAction> t1_actions = p1.getActions();
        vector<TankAction> t2_actions = p2.getActions();
        board.moveTanksRandomly();
        c_handler.handleCollisions(board);
        d.draw();
        if (this->concludeGame()){
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    logTie();
    
};