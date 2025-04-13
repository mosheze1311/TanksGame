#include "GameManager.h"
#include "../GameDrawer/GameDrawer.h"
#include <thread>
#include <chrono>
#include "../Logger/Logger.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

GameManager::GameManager(GameBoard &board, Player p1, Player p2, string output_file) : board(board) {};

int GameManager::getRemainingTurns() const
{
    return this->remaining_turns;
}

void GameManager::logWin(bool is_player1_winner)
{
    std::string winner = is_player1_winner ? "Player1 is the winner!" : "Player2 is the winner!";
    Logger::output().logInfo("The winner is: " + winner);
}

void GameManager::logTie()
{
    Logger::output().logInfo("The game is tied!");
}

void GameManager::logAction(Tank *tank, TankAction action, bool is_valid)
{
    std::string status = is_valid ? "Valid" : "Invalid";

    Logger::output().logInfo(
        status + " action: Tank type '" +
        std::string(1, static_cast<char>(tank->getObjectType())) +
        "' tried action number '" + std::to_string(static_cast<int>(action)) + "'");
}

void GameManager::performPlayerActionsOnBoard(vector<pair<Tank *, TankAction>> actions)
{
    for (pair<Tank *, TankAction> action_pair : actions)
    {
        bool is_valid = action_pair.first->action(action_pair.second);
        logAction(action_pair.first, action_pair.second, is_valid);
    }
}

bool GameManager::concludeGame()
{

    int p1_tanks = board.getGameObjectCount(GameObjectType::tank1);
    int p2_tanks = board.getGameObjectCount(GameObjectType::tank2);

    if (p1_tanks == 0 && p2_tanks == 0)
    {
        logTie();
        return true;
    }
    if (p1_tanks == 0)
    {
        logWin(false);
        return true;
    }
    if (p2_tanks == 0)
    {
        logWin(true);
        return true;
    }
    if (board.getTotalRemainingShells() <= 0)
    {
        if (this->getRemainingTurns() == 0)
        {
            logTie();
            return true;
        }
        this->remaining_turns--;
    }
    return false;
}

void GameManager::play()
{
    GameDrawer d(this->board);
    d.draw();
    GameCollisionHandler c_handler(this->board);
    while (true)
    {
        // TODO: this is a temporary logic for moving shells. should re-write correctly.
        for (int i = 0; i < 2; i++)
        {
            for (GameObject *shell : board.getGameObjects(GameObjectType::shell))
            {
                ((Shell *)shell)->action();
            }
            if(i==0){
                c_handler.handleCollisions(board);
                d.draw();
                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        }
        vector<TankAction> t1_actions = p1.getActions();
        vector<TankAction> t2_actions = p2.getActions();
        // board.moveTanksRandomly();
        ((Tank *)board.getGameObjects(GameObjectType::tank1)[0])->action(TankAction::FIRE);
        ((Tank *)board.getGameObjects(GameObjectType::tank2)[0])->action(TankAction::FIRE);
        c_handler.handleCollisions(board);
        d.draw();
        if (this->concludeGame())
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
};