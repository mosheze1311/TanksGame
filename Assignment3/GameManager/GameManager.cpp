#include "GameManager.h"

#include "../UserCommon/Config/ConfigReader.h"
#include "../UserCommon/GameCollisionHandler/GameCollisionHandler.h"
#include "../UserCommon/Utils/Timestamp.h"
#include "../UserCommon/Utils/ActionRequestUtils.h"
#include "../UserCommon/Utils/GameResultUtils.h"

using namespace GameManager_211388913_322330820;
// === Constructor === //
GameManager::GameManager(bool verbose) : verbose(verbose) {};

// === Prepare Run Functions === //
void GameManager::prepareForRun(size_t map_width, size_t map_height,
                                const SatelliteView &map,
                                size_t max_steps, size_t num_shells,
                                Player &player1, Player &player2,
                                TankAlgorithmFactory player1TankAlgoFactory,
                                TankAlgorithmFactory player2TankAlgoFactory)
{

    // Init output log file
    this->initOutputFile();

    // Init board and game details
    this->board.initFromDetails(map, map_width, map_height, max_steps, num_shells);
    this->remaining_steps = this->board.getMaxSteps();
    this->applyShellsLimitRuleOnRemainingSteps();

    // Init players map
    this->players_map[1] = &player1;
    this->players_map[2] = &player2;

    // Init algorithmFactoriesMap
    this->algo_factories_map[1] = player1TankAlgoFactory;
    this->algo_factories_map[2] = player2TankAlgoFactory;

    // Init algorithms based on board
    this->createAlgorithms();
}

void GameManager::createAlgorithms()
{
    std::map<size_t, int> algorithm_indexes_tracker{}; // maps a player index to next created algorithm index
    for (Tank *t : board.getAllTanksOrderedByCell())
    {
        int player_idx = GameObjectTypeUtils::tankTypeToPlayerIndex(t->getObjectType());
        int algorithm_idx = algorithm_indexes_tracker[player_idx]++; // ++ also increase count for next creation

        TankAlgorithmFactory algoFactory = this->algo_factories_map.at(player_idx);
        std::pair tank_algo_pair = {t, algoFactory(player_idx, algorithm_idx)};
        initial_tank_algorithm_pairs.push_back(std::move(tank_algo_pair));
    }
}

// === Getters === //
size_t GameManager::getRemainingSteps() const
{
    return this->remaining_steps;
}

std::vector<GameObjectType> GameManager::getActiveTankTypes(const std::map<GameObjectType, size_t> players_tanks_count) const
{

    std::vector<GameObjectType> tanks_types_alive;
    for (auto &[type, tank_count] : players_tanks_count)
    {
        if (tank_count > 0)
            tanks_types_alive.push_back(type);
    }

    return tanks_types_alive;
}

// === Setters === //
void GameManager::setRemainingSteps(int num_steps)
{
    this->remaining_steps = num_steps;
}

void GameManager::setGameResult(size_t winner, GameResult::Reason reason, const std::map<GameObjectType, size_t> &players_tanks_count)
{
    this->game_result.winner = winner;
    this->game_result.rounds = this->board.getMaxSteps() - this->getRemainingSteps();
    this->game_result.reason = reason;

    size_t n_players = players_tanks_count.size();
    this->game_result.remaining_tanks = std::vector<size_t>(n_players);
    for (auto &[tank_type, count] : players_tanks_count)
    {
        size_t player_id = GameObjectTypeUtils::tankTypeToPlayerIndex(tank_type);
        this->game_result.remaining_tanks[player_id - 1] = count;
    }
    this->game_result.gameState = std::make_unique<BoardSatelliteView>(this->TakeSatelliteImage());
}

// === Log Functions === //
void GameManager::initOutputFile()
{
    this->output_file_name = "output_GameManager_211388913_322330820" + getTimestampForNow() + ".txt";
}

void GameManager::writeToOutputFile(const std::string &text) const
{
    if (!verbose)
    {
        return;
    }

    Logger::output(this->output_file_name).log(text);
}

void GameManager::logStepActions(const std::map<int, ActionRequest> &actions, std::vector<bool> is_valid_action) const
{
    size_t n = initial_tank_algorithm_pairs.size();
    for (size_t i = 0; i < n; ++i)
    {
        bool coma_in_log = i != n - 1;
        // no action was requested - means tank was dead at beginning of turn
        auto iter = actions.find(i);
        if (iter == actions.end())
        {
            logKilled(coma_in_log);
            continue;
        }

        Tank *t = initial_tank_algorithm_pairs[i].first;
        bool is_killed = !(this->board.isObjectOnBoard(t));
        bool is_valid = is_valid_action[i];
        ActionRequest action = iter->second;

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

    this->writeToOutputFile(action_log);

    if (coma)
        logComa();
}

void GameManager::logKilled(bool coma) const
{
    this->writeToOutputFile("killed");

    if (coma)
        logComa();
}

void GameManager::logComa() const
{
    this->writeToOutputFile(", ");
}

void GameManager::logEndOfStep() const
{
    this->writeToOutputFile("\n");
}

void GameManager::logEndOfGameLine() const
{
    this->writeToOutputFile(GameResultUtils::endOfGameLine(this->game_result) + "\n");
}

// === Gameplay Functions === //
std::vector<bool> GameManager::performActionsOnBoard(std::map<int, ActionRequest> actions, BoardSatelliteView &sat_view, GameCollisionHandler &c_handler)
{
    size_t n = initial_tank_algorithm_pairs.size();
    std::vector<bool> is_valid_action(n);
    for (size_t i = 0; i < n; ++i)
    {
        auto iter = actions.find(i);

        // if no action request -> tank was dead in the begining of turn
        if (iter == actions.end())
        {
            continue;
        }

        Tank *tank = initial_tank_algorithm_pairs[i].first;
        ActionRequest action = iter->second;

        if (action == ActionRequest::GetBattleInfo)
        {
            Player *player = players_map[GameObjectTypeUtils::tankTypeToPlayerIndex(tank->getObjectType())];

            TankAlgorithm &algorithm = *(initial_tank_algorithm_pairs[i].second);
            BoardCell tank_location = *(board.getObjectLocation(tank));

            sat_view.setCallerTankLocation(tank_location);
            player->updateTankWithBattleInfo(algorithm, sat_view);
        }

        bool is_valid = tank->playTankRound(action);
        is_valid_action[i] = is_valid;
    }

    c_handler.handleCollisions();

    return is_valid_action;
}

bool GameManager::concludeGame()
{
    std::map<GameObjectType, size_t> players_tanks_count = board.getTanksCountPerType();
    std::vector<GameObjectType> tanks_types_alive = this->getActiveTankTypes(players_tanks_count);

    this->applyShellsLimitRuleOnRemainingSteps();

    if (tanks_types_alive.size() == 1)
    {
        size_t winner = GameObjectTypeUtils::tankTypeToPlayerIndex(tanks_types_alive[0]);
        setGameResult(winner, GameResult::Reason::ALL_TANKS_DEAD, players_tanks_count);
        return true;
    }

    if (tanks_types_alive.empty())
    {
        setGameResult(0, GameResult::Reason::ALL_TANKS_DEAD, players_tanks_count);
        return true;
    }

    if (this->getRemainingSteps() == 0)
    {
        setGameResult(0, this->are_steps_limited_by_shells ? GameResult::Reason::MAX_STEPS : GameResult::Reason::MAX_STEPS,
                      players_tanks_count);
        return true;
    }

    return false;
}

void GameManager::moveShellsOnce()
{
    const auto gameObjs = board.getGameObjects(GameObjectType::SHELL);
    std::vector<Shell *> shells;
    std::transform(gameObjs.begin(), gameObjs.end(), std::back_inserter(shells),
                   [](GameObject *obj)
                   { return static_cast<Shell *>(obj); });

    for (Shell *shell : shells)
    {
        shell->advance();
    }
}

void GameManager::moveShells(int times, GameCollisionHandler &c_handler)
{
    for (int i = 0; i < times; ++i)
    {
        this->moveShellsOnce();
        c_handler.handleCollisions();
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

std::map<int, ActionRequest> GameManager::requestAlgorithmsActions()
{
    std::map<int, ActionRequest> actions; // using map because some tanks may not be alive anymore
    for (size_t i = 0; i < initial_tank_algorithm_pairs.size(); ++i)
    {
        auto &[tank, algorithm_ptr] = initial_tank_algorithm_pairs[i];
        if (!board.isObjectOnBoard(tank))
            continue;

        ActionRequest algorithm_action = algorithm_ptr->getAction();
        actions[i] = algorithm_action;
    }

    return actions;
}

// === Helper Functions === //
void GameManager::applyShellsLimitRuleOnRemainingSteps()
{
    if (this->board.getTotalRemainingShells() == 0)
    {
        if (ConfigReader::getConfig().getStepsAfterShellsEnd() < this->getRemainingSteps())
        {
            this->are_steps_limited_by_shells = true;
            this->setRemainingSteps(ConfigReader::getConfig().getStepsAfterShellsEnd());
        }
    }
}

// === Public Functions === //
GameResult GameManager::run(size_t map_width, size_t map_height,
                            const SatelliteView &map,
                            size_t max_steps, size_t num_shells,
                            Player &player1, Player &player2,
                            TankAlgorithmFactory player1_tank_algo_factory,
                            TankAlgorithmFactory player2_tank_algo_factory)
{

    this->prepareForRun(map_width, map_height, map, max_steps,
                        num_shells, player1, player2,
                        player1_tank_algo_factory,
                        player2_tank_algo_factory);

    // running game loop
    GameCollisionHandler c_handler(this->board);
    while (true)
    {
        // check for winner / tie
        if (this->concludeGame())
            break;

        BoardSatelliteView sat_view = this->TakeSatelliteImage();

        std::map<int, ActionRequest> actions = requestAlgorithmsActions();
        std::vector<bool> is_valid_action = this->performActionsOnBoard(actions, sat_view, c_handler);

        this->moveShells(ConfigReader::getConfig().getShellsSpeed(), c_handler);

        logStepActions(actions, is_valid_action);
        this->advanceStepsClock();
    }
    this->logEndOfGameLine();

    return std::move(this->game_result);
}