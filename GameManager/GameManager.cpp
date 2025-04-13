# include "GameManager.h"
#include "../GameDrawer/GameDrawer.h"
#include <thread>
#include <chrono>
#include "../Logger/Logger.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

GameManager::GameManager(GameBoard board, Player p1, Player p2, string output_file): board(board) {};

// bool GameManager::isUserActionValid(std::pair<Tank*, TankAction> action_pair) const {
//     Tank* tank = action_pair.first;
//     TankAction action = action_pair.second;

//     if (!tank) return false;

//     int shootCooldown = tank->getShootCooldown();
//     int backwardWait = tank->getBackwardWait();
//     int shells = tank->getShells();

//     switch (action) {
//         case TankAction::NOTHING:
//             return true;

//         case TankAction::FORWARD:
//         case TankAction::TURN45LEFT:
//         case TankAction::TURN45RIGHT:
//         case TankAction::TURN90LEFT:
//         case TankAction::TURN90RIGHT:
//             return backwardWait < 0;

//         case TankAction::BACKWARD:
//             return backwardWait <= 0; 

//         case TankAction::FIRE:
//             return backwardWait < 0 && shootCooldown == 0 && shells > 0;

//         default:
//             return false;
//     }
// }


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

void GameManager::play(){
    GameDrawer d(board);
    GameCollisionHandler c_handler(board);
    while (true)
    {
        vector<TankAction> t1_actions = p1.getActions();
        vector<TankAction> t2_actions = p2.getActions();
        board.moveTanksRandomly();
        c_handler.handleCollisions(board);
        d.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    
};