#include "GameManager.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../Utils/ActionRequestutils.h"

//=== Getters ===
int GameManager::getRemainingSteps() const
{
    return this->remaining_steps;
}

vector<GameObjectType> GameManager::getActiveTankTypes(map<GameObjectType, size_t> players_tanks_count) const{

    vector<GameObjectType> tanks_types_alive;
    for (auto &[type, tank_count] : players_tanks_count)
    {
        if (tank_count > 0)
            tanks_types_alive.push_back(type);
    }

    return tanks_types_alive;
}

//=== Setters ===
void GameManager::setRemainingSteps(int num_steps){
    this->remaining_steps = num_steps;
}

//=== Log Functions ===
void GameManager::logWin(int winner, int remaining_tanks) const
{
    string winning_text = "Player " + std::to_string(winner) + " won with " + std::to_string(remaining_tanks) + " tanks still alive";
    Logger::output(output_file_name).logLine(winning_text);
}

void GameManager::logZeroTanksTie() const
{
    logTie("both players have zero tanks");
}

void GameManager::logMaxStepsTie() const
{

    std::map<GameObjectType, size_t> players_tanks_count = board.getTanksCountPerType();
    std::string reason = "reached max steps = " + std::to_string(this->board.getMaxSteps());

    for (const auto &[type, count] : players_tanks_count)
    {
        int playerId = GameObjectTypeUtils::tankTypeToPlayerIndex(type);
        reason += ", player " + std::to_string(playerId) + " has " + std::to_string(count) + " tanks";
    }

    logTie(reason);
}

void GameManager::logZeroShellsTie() const
{
    logTie("both players have zero shells for " + std::to_string(this->steps_after_shells_end) + " steps");
}

void GameManager::logTie(string reason) const
{
    Logger::output(output_file_name).logLine("Tie, " + reason);
}

void GameManager::logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const
{
    Logger::output(output_file_name).log(ActionRequestUtils::actionToString(action));

    if (is_killed)
        Logger::output(output_file_name).log("(killed)");

    else if (!is_valid)
        Logger::output(output_file_name).log("(ignored)");

    if (coma)
        logComa();
}

void GameManager::logKilled(bool coma) const
{
    Logger::output(output_file_name).log("killed");
    if (coma)
        logComa();
}

void GameManager::logComa() const
{

    Logger::output(output_file_name).log(", ");
}

void GameManager::logEndOfStep() const
{
    Logger::output(output_file_name).logLine("");
}

//=== Gameplay Functions ===
void GameManager::performActionsOnBoard(map<int, ActionRequest> actions)
{
    size_t n = tanks.size();
    for (size_t i = 0; i < n; ++i)
    {
        auto iter = actions.find(i);

        Tank *tank = tanks[i];
        ActionRequest action = iter->second;
        bool is_killed;
        bool coma = i != n - 1;

        // if no action request -> tank was dead in the begining of turn
        if (iter == actions.end())
        {
            this->logKilled(coma);
            continue;
        }

        // log action: tank was killed during step (before performing action)
        if ((is_killed = !this->board.isObjectOnBoard(tank)))
        {
            logAction(action, true, is_killed, coma);
            continue;
        }

        // log action: if tank has invalid request log (ignore); else, log normally.
        bool is_valid = tank->playTankRound(action);
        logAction(action, is_valid, is_killed, coma);
    }

    this->logEndOfStep();
}

bool GameManager::concludeGame()
{
    map<GameObjectType, size_t> players_tanks_count = board.getTanksCountPerType();
    vector<GameObjectType> tanks_types_alive = this->getActiveTankTypes(players_tanks_count);

    if (this->board.getTotalRemainingShells() == 0)
    {
        if (steps_after_shells_end < this->getRemainingSteps())
        {
            are_steps_limited_by_shells = true;
            this->setRemainingSteps(steps_after_shells_end);
        }
    }

    if (tanks_types_alive.size() == 1)
    {
        this->logWin(GameObjectTypeUtils::tankTypeToPlayerIndex(tanks_types_alive[0]), players_tanks_count[tanks_types_alive[0]]);
        return true;
    }

    if (tanks_types_alive.empty())
    {
        this->logZeroTanksTie();
        return true;
    }

    if (this->getRemainingSteps() == 0)
    {
        this->are_steps_limited_by_shells ? this->logZeroShellsTie() : this->logMaxStepsTie();
        return true;
    }

    return false;
}

void GameManager::moveShellsOnce()
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

void GameManager::moveShells(int times, GameCollisionHandler &c_handler, GameDrawer &d)
{
    for (int i = 0; i < times; ++i)
    {
        this->moveShellsOnce();
        c_handler.handleCollisions(board);
        d.draw();
    }
}

void GameManager::advanceStepsClock()
{
    this->remaining_steps--;
}

BoardSatelliteView GameManager::TakeSatelliteImage()
{
    return BoardSatelliteView(this->board);
}

map<int, ActionRequest> GameManager::requestAlgorithmsActions()
{
    map<int, ActionRequest> actions;
    for (size_t i = 0; i < tanks.size(); ++i)
    {
        Tank *tank = tanks[i];
        if (!board.isObjectOnBoard(tank))
            continue;

        ActionRequest algorithm_action = this->algorithms[i]->getAction();
        actions[i] = algorithm_action;
    }

    return actions;
}

//=== Public Functions ===
//=== Constructor ===
GameManager::GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

void GameManager::run(DrawingType dt)
{
    GameCollisionHandler c_handler(this->board);
    GameDrawer d(this->board, dt);

    d.draw();

    while (true)
    {
        BoardSatelliteView sat_view = this->TakeSatelliteImage();
        map<int, ActionRequest> actions = requestAlgorithmsActions();
        this->advanceStepsClock();

        // move shells - TODO: make speed a configured paramter instead of static number
        this->moveShells(2, c_handler, d);

        // perform algorithms action requests
        this->performActionsOnBoard(actions);

        c_handler.handleCollisions(board);
        d.draw();

        // check for winner / tie
        if (this->concludeGame())
            break;
    }
}

bool GameManager::readBoard(std::string input_file_path)
{
    return BoardFactory::initGameBoardFromFile(this->board, input_file_path);
}
