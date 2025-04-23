#include "GameManager.h"
#include "../GameDrawer/GameDrawer.h"
#include <thread>
#include <chrono>
#include "../Logger/Logger.h"


GameManager::GameManager(GameBoard &board, Player p1, Player p2) : board(board), p1(p1), p2(p2) {};

int GameManager::getRemainingTurns() const
{
    return this->remaining_turns;
}

void GameManager::logWin(bool is_player1_winner, string reason)
{
    std::string winner = is_player1_winner ? "Player 1!" : "Player 2!";
    Logger::output().logInfo("The winner is: " + winner + " " + reason);
}

void GameManager::logTie(string reason)
{
    Logger::output().logInfo("The game is tied! " + reason);
}

void GameManager::logAction(Tank *tank, TankAction action, bool is_valid)
{
    std::string status = is_valid ? "Valid" : "Invalid";

    Logger::output().logInfo(
        status + " action: Tank type '" +
        std::string(1, static_cast<char>(tank->getObjectType())) +
        "' tried action number '" + std::to_string(static_cast<int>(action)) + "'");
}

void GameManager::performPlayerActionsOnBoard(map<Tank *, TankAction> actions)
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
        logTie("All tanks exploded");
        return true;
    }
    if (p1_tanks == 0)
    {
        logWin(false, "All player's 1 tanks exploded!");
        return true;
    }
    if (p2_tanks == 0)
    {
        logWin(true, "All player's 2 tanks exploded!");
        return true;
    }
    if (board.getTotalRemainingShells() <= 0)
    {
        if (this->getRemainingTurns() == 0)
        {
            logTie("No more shells...");
            return true;
        }
        this->remaining_turns--;
    }
    return false;
}

void GameManager::moveShells(GameCollisionHandler& c_handler, GameDrawer d)
 // Moves all shells ny 2 steps each turn
{
    const auto& gameObjs = board.getGameObjects(GameObjectType::shell);
    std::vector<Shell*> shells;

    std::transform(gameObjs.begin(), gameObjs.end(), std::back_inserter(shells),
               [](GameObject* obj) { return static_cast<Shell*>(obj); });

    for (int i = 0; i < 2; i++)
    {
        for (Shell *shell : shells){
            shell->action();
        }
        if ( i == 0){
            c_handler.handleCollisions(board);
            d.draw();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } 
}

void GameManager::play(DrawingType dt)
{
    GameDrawer d(this->board, dt);
    d.draw();
    
    GameCollisionHandler c_handler(this->board);
    while (true)
    {
        this->moveShells(c_handler, d);

        map<Tank*, TankAction> t1_actions = this->p1.getActionsFromPlayer(this->board);
        map<Tank *, TankAction> t2_actions = this->p2.getActionsFromPlayer(this->board);
        this->performPlayerActionsOnBoard(t1_actions);
        this->performPlayerActionsOnBoard(t2_actions);
        
        c_handler.handleCollisions(board);

        d.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        if (this->concludeGame())
        {
            break;
        }
    }
};

// TODO: delete if not necessary
void GameManager::simulateGame(){
    GameDrawer d(this->board);
    GameCollisionHandler c_handler(this->board);

    d.draw();
    while (true)
    {
        this->moveShells(c_handler, d);

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
}