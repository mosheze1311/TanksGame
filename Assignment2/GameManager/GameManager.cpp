#include "GameManager.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

//=== Constructor ===
GameManager::GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

//=== Getters ===
int GameManager::getRemainingTurns() const
{
    return this->remaining_turns;
}

//=== Log Functions ===
void GameManager::logWin(bool is_player1_winner, string reason)
{
    std::string winner = is_player1_winner ? "Player 1!" : "Player 2!";
    Logger::output(output_file_name).logInfo("The winner is: " + winner + " " + reason);
}

void GameManager::logTie(string reason)
{
    Logger::output(output_file_name).logInfo("The game is tied! " + reason);
}

void GameManager::logAction(Tank *tank, TankAction action, bool is_valid)
{
    std::string status = is_valid ? "Valid" : "Invalid";

    Logger::output(output_file_name).logInfo(status + " action: Tank type '" + std::string(1, static_cast<char>(tank->getObjectType())) + "' tried action " + TankActionUtil::getName(action) + "");
}

//=== Gameplay Functions ===
void GameManager::performPlayerActionsOnBoard(map<Tank *, TankAction> actions)
{
    for (pair<Tank *, TankAction> action_pair : actions)
    {
        bool is_valid = action_pair.first->playTankRound(action_pair.second);
        logAction(action_pair.first, action_pair.second, is_valid);
    }
}

bool GameManager::concludeGame()
{
    int p1_tanks = board.getGameObjectCount(GameObjectType::TANK1);
    int p2_tanks = board.getGameObjectCount(GameObjectType::TANK2);

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

//=== Public Function ===
void GameManager::run(DrawingType dt)
{
    // TODO: need to modify this function logic.
    GameCollisionHandler c_handler(this->board);
    GameDrawer d(this->board, dt);
    d.draw();

    while (true)
    {
        for (int i = 0; i < 2; i++)
        {
            this->moveShells();
            c_handler.handleCollisions(board);
            d.draw();
        }

        // TODO: handle TanksAlgorithmsLogic here.
        
        // map<Tank *, TankAction> t1_actions = this->p1.getActionsFromPlayer(this->board);
        // this->performPlayerActionsOnBoard(t1_actions);
        // map<Tank *, TankAction> t2_actions = this->p2.getActionsFromPlayer(this->board);
        // this->performPlayerActionsOnBoard(t2_actions);
        c_handler.handleCollisions(board);
        d.draw();

        if (this->concludeGame())
        {
            break;
        }
    }}

bool GameManager::readBoard(std::string input_file_path)
{
    return BoardFactory::initGameBoardFromFile(this->board, input_file_path);
}
