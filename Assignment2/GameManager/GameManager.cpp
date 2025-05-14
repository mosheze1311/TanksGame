#include "GameManager.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../Utils/ActionRequestutils.h"

//=== Getters ===
int GameManager::getRemainingTurns() const
{
    return this->remaining_turns;
}

//=== Log Functions ===
void GameManager::logWin(int winner, int remaining_tanks)
{
    string winning_text = "Player " + std::to_string(winner) + " won with " + std::to_string(remaining_tanks) + " tanks still alive";
    Logger::output(output_file_name).logInfo(winning_text);
}

void GameManager::logZeroTanksTie(){
    logTie("both players have zero tanks");
    
}

void GameManager::logMaxStepsTie(){
    // TODO: allow using up to 9 players
    int p1_tanks = this->board.getGameObjectCount(GameObjectType::TANK1);
    int p2_tanks = this->board.getGameObjectCount(GameObjectType::TANK2);
    logTie("reached max steps = " + std::to_string(this->board.getMaxSteps()) + ", " +
           +"player 1 has " + std::to_string(p1_tanks) + " tanks, " +
           +"player 2 has " + std::to_string(p2_tanks) + " tanks");
}

void GameManager::logTie(string reason)
{
    Logger::output(output_file_name).logInfo("Tie, " + reason);
}

void GameManager::logAction(Tank *tank, ActionRequest action, bool is_valid)
{
    std::string status = is_valid ? "Valid" : "Invalid";

    Logger::output(output_file_name).logInfo(status + " action: Tank type '" + std::string(1, static_cast<char>(tank->getObjectType())) + "' tried action " + ActionRequestUtils::actionToString(action) + "");
}

//=== Gameplay Functions ===
void GameManager::performActionsOnBoard(map<Tank *, ActionRequest> actions)
{
    for (pair<Tank *, ActionRequest> action_pair : actions)
    {
        bool is_valid = action_pair.first->playTankRound(action_pair.second);
        logAction(action_pair.first, action_pair.second, is_valid);
    }
}

bool GameManager::concludeGame()
{
    // TODO: allow using up to 9 players
    int p1_tanks = board.getGameObjectCount(GameObjectType::TANK1);
    int p2_tanks = board.getGameObjectCount(GameObjectType::TANK2);

    if (p1_tanks == 0 && p2_tanks == 0)
    {
        this->logZeroTanksTie();
        return true;
    }
    if (p2_tanks == 0)
    {
        logWin(true, p1_tanks);
        return true;
    }
    if (p1_tanks == 0)
    {
        logWin(false, p2_tanks);
        return true;
    }
    if (this->getRemainingTurns() == 0)
    {
        this->logMaxStepsTie();
        return true;
    }
    return false;
}

void GameManager::moveShells()
{
    const auto &gameObjs = board.getGameObjects(GameObjectType::SHELL);
    std::vector<Shell *> shells;
    std::transform(gameObjs.begin(), gameObjs.end(), std::back_inserter(shells),
                   [](GameObject *obj)
                   { return static_cast<Shell *>(obj); });

    for (Shell *shell : shells)
    {
        shell->advance();
    }
}

BoardSatelliteView GameManager::TakeSatelliteImage() 
{
    return BoardSatelliteView(this->board);
}

//=== Public Functions ===
//=== Constructor ===
GameManager::GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

void GameManager::run(DrawingType dt)
{
    // TODO: need to modify this function logic.
    GameCollisionHandler c_handler(this->board);
    GameDrawer d(this->board, dt);
    d.draw();

    while (true)
    {
        BoardSatelliteView sat_view = this->TakeSatelliteImage();

        // move shells - TODO: make speed a configured paramter instead of static number
        for (int i = 0; i < 2; i++)
        {
            this->moveShells();
            c_handler.handleCollisions(board);
            d.draw();
        }

        // perfrom tanks algorithms actions
        map<Tank *, ActionRequest> actions;
        for (auto &[tank, algorithm] : tanks_algorithms)
        {

            ActionRequest algorithm_action = algorithm->getAction();
            actions[tank] = algorithm_action;
        }
        this->performActionsOnBoard(actions);

        c_handler.handleCollisions(board);
        d.draw();

        // check for winner / tie
        if (this->concludeGame())
        {
            break;
        }
    }
}

bool GameManager::readBoard(std::string input_file_path)
{
    return BoardFactory::initGameBoardFromFile(this->board, input_file_path);
}
