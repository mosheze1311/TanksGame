#pragma once

#include "../common/Player.h"
#include "../common/TankAlgorithm.h"
#include "../common/AbstractGameManager.h"

#include "../UserCommon/BoardSatelliteView/BoardSatelliteView.h"
#include "../UserCommon/GameCollisionHandler/GameCollisionHandler.h"
#include "../UserCommon/GameBoard/GameBoard.h"
#include "../UserCommon/Logger/Logger.h"
#include "../UserCommon/Utils/GameObjectTypeUtils.h"

#include <filesystem> // for splitting the file_path to name only
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <type_traits>
#include <vector>

namespace GameManager_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    class GameManager : public AbstractGameManager
    {
    private:
        // === Attributes === //
        bool verbose;
        GameBoard board;

        size_t remaining_steps;
        bool are_steps_limited_by_shells = false;

        std::map<int, TankAlgorithmFactory> algo_factories_map;
        std::vector<std::pair<Tank *, std::unique_ptr<TankAlgorithm>>> initial_tank_algorithm_pairs;
        std::map<int, Player *> players_map;

        std::string output_file_name;

        GameResult game_result;
        // === Getters === //
        size_t getRemainingSteps() const; // private getter for readability
        std::vector<GameObjectType> getActiveTankTypes(const std::map<GameObjectType, size_t> players_tanks_count) const;

        // === Setters === //
        void setRemainingSteps(int num_shells);
        void setGameResult(size_t winner, GameResult::Reason reason, const std::map<GameObjectType, size_t>& players_tanks_count);

        // === Gameplay Function === //
        void advanceStepsClock();
        BoardSatelliteView TakeSatelliteImage(); // Returns an updated SatelliteView object at the start of the turn.
        std::map<int, ActionRequest> requestAlgorithmsActions();
        std::vector<bool> performActionsOnBoard(std::map<int, ActionRequest> actions, BoardSatelliteView &sat_view, GameCollisionHandler &c_handler);
        void moveShells(int times, GameCollisionHandler &c_handler);
        void moveShellsOnce(); // Move shells acoording to their direction
        bool concludeGame();   // Checks if a game is finsihed in a specific turn

        // === Log Functions === //
        void initOutputFile();
        void writeToOutputFile(const std::string &text) const;

        void logStepActions(const std::map<int, ActionRequest> &actions, std::vector<bool> is_valid_action) const;
        void logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const;
        void logKilled(bool coma) const;
        void logComa() const;
        void logEndOfStep() const;
        void logEndOfGameLine() const;

        // === Prepare Run Functions === //
        void prepareForRun(size_t map_width, size_t map_height,
                           const SatelliteView &map,
                           size_t max_steps, size_t num_shells,
                           Player &player1, Player &player2,
                           TankAlgorithmFactory player1_tank_algo_factory,
                           TankAlgorithmFactory player2_tank_algo_factory);
        
        void createAlgorithms();

        // === Helper Functions === //
        void applyShellsLimitRuleOnRemainingSteps();

    public:
        // === Constructor for Temp Factory Objects === //
        GameManager(bool verbose);

        // === Destructor === //
        ~GameManager() = default;

        // === Copy (Deleted) === //
        GameManager(const GameManager &) = delete;
        GameManager &operator=(const GameManager &) = delete;

        // === Move (Deleted) === //
        GameManager(GameManager &&) = delete;
        GameManager &operator=(GameManager &&) = delete;

        // === Public API === //
        // === Runs Game === //
        GameResult run(size_t map_width, size_t map_height,
                       const SatelliteView &map, // <= assume it is a snapshot, NOT updated
                       size_t max_steps, size_t num_shells, Player &player1, Player &player2,
                       TankAlgorithmFactory player1_tank_algo_factory,
                       TankAlgorithmFactory player2_tank_algo_factory) override;
    };

};