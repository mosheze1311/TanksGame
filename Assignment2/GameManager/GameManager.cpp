#include "../config.h"

#include "GameManager.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../Utils/ActionRequestutils.h"

//=== Prepare Run Functions ===
void GameManager::prepareForRun()
{
    // set manager game parameters
    this->game_total_tanks = board.getAllTanksOrderedByCell(); // TODO: make sure tanks are ordered correctly;
    this->remaining_steps = this->board.getMaxSteps();
    this->applyShellsLimitRuleOnRemainingSteps();

    // initiate players and algorithms based on board
    this->createPlayers();
    this->createAlgorithms();
}

void GameManager::createPlayers()
{   
    std::map<GameObjectType, size_t> tanks_counters = this->board.getTanksCountPerType();
    for (int i = 1; i <= 9; ++i)
    {
        GameObjectType tank_type = GameObjectTypeUtils::playerIndexToTankType(i);
        size_t tanks_count = tanks_counters[tank_type];
        if (tanks_count == 0)
            continue;

        this->players_map[i] = player_factory->create(i, this->board.getWidth(), this->board.getHeight(), this->board.getMaxSteps(), this->board.getTanksNumShells());
    }
}

void GameManager::createAlgorithms()
{
    std::map<size_t, size_t> algorithm_indexes_tracker;
    for (Tank *t : game_total_tanks)
    {
        size_t player_idx = GameObjectTypeUtils::tankTypeToPlayerIndex(t->getObjectType());
        size_t algorithm_idx = algorithm_indexes_tracker[player_idx]++;
        Logger::runtime().logLine("create algorithm " + std::to_string(algorithm_idx) + " for player " + std::to_string(player_idx));
        algorithms.push_back(this->tank_algorithm_factory->create(player_idx, algorithm_idx));
    }
}

//=== Getters ===
size_t GameManager::getRemainingSteps() const
{
    return this->remaining_steps;
}

std::vector<GameObjectType> GameManager::getActiveTankTypes(map<GameObjectType, size_t> players_tanks_count) const
{

    std::vector<GameObjectType> tanks_types_alive;
    for (auto &[type, tank_count] : players_tanks_count)
    {
        if (tank_count > 0)
            tanks_types_alive.push_back(type);
    }

    return tanks_types_alive;
}

//=== Setters ===
void GameManager::setRemainingSteps(int num_steps)
{
    this->remaining_steps = num_steps;
}

//=== Log Functions ===
void GameManager::setOutputFile(std::string &input_file_path)
{
    std::string input_file_name = std::filesystem::path(input_file_path).stem().string(); // this is a standard library function the return the name of a file
    this->output_file_name = "output_" + input_file_name + ".txt";
}

void GameManager::logStepActions(std::map<int, ActionRequest> actions, std::vector<bool> is_valid_action) const
{
    size_t n = game_total_tanks.size();
    for (size_t i = 0; i < n; ++i)
    {
        bool coma_in_log = i != n - 1;
        // no action was requested - means tank was dead at beginning of turn
        if (actions.find(i) == actions.end())
        {
            logKilled(coma_in_log);
            continue;
        }

        Tank *t = game_total_tanks[i];
        bool is_killed = !(this->board.isObjectOnBoard(t)); // TODO: perhaps create a Manager function that checks if tank is alive
        bool is_valid = is_valid_action[i];
        ActionRequest action = actions[i];

        logAction(action, is_valid, is_killed, coma_in_log);
    }
    logEndOfStep();
}

void GameManager::logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const
{
    std::string action_log = ActionRequestUtils::actionToString(action);
    if (!is_valid)
        action_log += " (ignored)";
    if (is_killed)
        action_log += " (killed)";

    Logger::output(output_file_name).log(action_log);

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

void GameManager::logWin(int winner, int remaining_tanks) const
{
    std::string winning_text = "Player " + std::to_string(winner) + " won with " + std::to_string(remaining_tanks) + " tanks still alive";
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
    logTie("both players have zero shells for " + std::to_string(steps_after_shells_end) + " steps");
}

void GameManager::logTie(std::string reason) const
{
    Logger::output(output_file_name).logLine("Tie, " + reason);
}

//=== Gameplay Functions ===
std::vector<bool> GameManager::performActionsOnBoard(map<int, ActionRequest> actions, BoardSatelliteView &sat_view, GameCollisionHandler &c_handler, GameDrawer &d)
{
    size_t n = game_total_tanks.size();
    std::vector<bool> is_valid_action(n);
    for (size_t i = 0; i < n; ++i)
    {
        auto iter = actions.find(i);

        // if no action request -> tank was dead in the begining of turn
        if (iter == actions.end())
        {
            continue;
        }

        Tank *tank = game_total_tanks[i];
        ActionRequest action = iter->second;

        // TODO: ActionRequest::GetBAttleInfo should either cancel/allow moving backwards - currently, skipping tank turn which is weird and make no sense...
        bool is_valid = true;
        if (action == ActionRequest::GetBattleInfo)
        {
            Player *player = players_map[GameObjectTypeUtils::tankTypeToPlayerIndex(tank->getObjectType())].get();

            TankAlgorithm &algorithm = *(algorithms[i]);
            BoardCell tank_location = *(board.getObjectLocation(tank));

            sat_view.setCallerTankLocation(tank_location);
            player->updateTankWithBattleInfo(algorithm, sat_view);
            tank->playTankRound(ActionRequest::DoNothing); // TODO: check - this is here to advance the turn of the tank

        }
        else
        {
            is_valid = tank->playTankRound(action);
        }
        is_valid_action[i] = is_valid;
    }

    c_handler.handleCollisions(this->board);
    d.draw();

    return is_valid_action;
}

bool GameManager::concludeGame()
{
    std::map<GameObjectType, size_t> players_tanks_count = board.getTanksCountPerType();
    std::vector<GameObjectType> tanks_types_alive = this->getActiveTankTypes(players_tanks_count);

    this->applyShellsLimitRuleOnRemainingSteps();

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
    for (size_t i = 0; i < game_total_tanks.size(); ++i)
    {
        Tank *tank = game_total_tanks[i];
        if (!board.isObjectOnBoard(tank))
            continue;

        ActionRequest algorithm_action = this->algorithms[i]->getAction();
        actions[i] = algorithm_action;
    }

    return actions;
}

//=== Helper Functions ===
void GameManager::applyShellsLimitRuleOnRemainingSteps()
{
    if (this->board.getTotalRemainingShells() == 0)
    {
        if (steps_after_shells_end < this->getRemainingSteps())
        {
            this->are_steps_limited_by_shells = true;
            this->setRemainingSteps(steps_after_shells_end);
        }
    }
}

//=== Public Functions ===
//=== Constructor ===
GameManager::GameManager(const std::shared_ptr<PlayerFactory> player_factory, const std::shared_ptr<TankAlgorithmFactory> tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

bool GameManager::readBoard(std::string input_file_path)
{
    // init board from file
    bool success = this->board.initFromFile(input_file_path);

    // set output to required file
    this->setOutputFile(input_file_path);

    return success;
}

// TODO: test for errors when calling 'run' before calling readBoard.
void GameManager::run(DrawingType dt)
{
    this->prepareForRun();
    GameCollisionHandler c_handler(this->board);
    GameDrawer d(this->board, dt);
    d.draw();

    while (true)
    {
        // TODO: which moves first? Tanks or Shells?  - currently tanks move first
        BoardSatelliteView sat_view = this->TakeSatelliteImage();
        map<int, ActionRequest> actions = requestAlgorithmsActions();
        this->advanceStepsClock();

        std::vector<bool> is_valid_action = this->performActionsOnBoard(actions, sat_view, c_handler, d);
        
        this->moveShells(shell_speed, c_handler, d);
        
        logStepActions(actions, is_valid_action);

        // check for winner / tie
        if (this->concludeGame())
            break;
    }
}