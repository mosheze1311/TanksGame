# include "GameManager.h"
#include "../GameDrawer/GameDrawer.h"
#include <thread>
#include <chrono>
#include "../Logger/Logger.h"

GameManager::GameManager(GameBoard board, Player p1, Player p2, string output_file): board(board) {};

bool GameManager::isUserActionValid(std::pair<Tank*, TankAction> action_pair) const {
    Tank* tank = action_pair.first;
    TankAction action = action_pair.second;

    if (!tank) return false;

    int shootCooldown = tank->getShootCooldown();
    int backwardWait = tank->getBackwardWait();
    int shells = tank->getShells();

    switch (action) {
        case TankAction::NOTHING:
            return true;

        case TankAction::FORWARD:
        case TankAction::TURN45LEFT:
        case TankAction::TURN45RIGHT:
        case TankAction::TURN90LEFT:
        case TankAction::TURN90RIGHT:
            return backwardWait < 0;

        case TankAction::BACKWARD:
            return backwardWait <= 0; 

        case TankAction::FIRE:
            return backwardWait < 0 && shootCooldown == 0 && shells > 0;

        default:
            return false;
    }
}


void GameManager::logInvalidAction(Tank* tank, TankAction action) {
    Logger::runtime().logError(
        "Invalid action: Tank type '" +
        std::string(1, static_cast<char>(tank->getObjectType())) +
        "' tried action number '" + std::to_string(static_cast<int>(action)) + "'"
    );
}

 void GameManager::performPlayerActionsOnBoard(vector<pair<Tank *, TankAction>> actions)
{
    for (pair<Tank*, TankAction> action_pair : actions)
    {
        // skip invalid actions
        if (! this->isUserActionValid(action_pair)){
            logInvalidAction(action_pair.first, action_pair.second);
            continue;
        }
        
        // TODO: perform the actions.
    }
}

void GameManager::play(){
    GameDrawer d(board);
    while (true)
    {
        vector<TankAction> t1_actions = p1.getActions();
        vector<TankAction> t2_actions = p2.getActions();
        board.moveTanksRandomly();
        d.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
};