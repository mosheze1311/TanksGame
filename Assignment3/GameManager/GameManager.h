#pragma once

#include "../common/PlayerFactory.h"
#include "../common/TankAlgorithmFactory.h"

#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../GameBoardInitializer/GameBoardInitializer.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameDrawer/GameDrawer.h"
#include "../GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include "../Utils/GameObjectTypeUtils.h"

#include <filesystem> // for splitting the file_path to name only
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <type_traits>
#include <vector>

template <typename T>
concept DerivedFromPlayerFactory = std::is_base_of<PlayerFactory, std::decay_t<T>>::value;

template <typename T>
concept DerivedFromTankAlgorithmFactory = std::is_base_of<TankAlgorithmFactory, std::decay_t<T>>::value;

class GameManager
{
private:
    // === Attributes === //
    GameBoard board;

    size_t remaining_steps;
    bool are_steps_limited_by_shells = false;

    const std::unique_ptr<PlayerFactory> player_factory;
    const std::unique_ptr<TankAlgorithmFactory> tank_algorithm_factory;

    std::vector<std::pair<Tank *, std::unique_ptr<TankAlgorithm>>> initial_tank_algorithm_pairs;
    std::map<int, std::unique_ptr<Player>> players_map;

    std::string output_file_name;

    // === Getters === //
    size_t getRemainingSteps() const; // private getter for readability
    std::vector<GameObjectType> getActiveTankTypes(std::map<GameObjectType, size_t> players_tanks_count) const;

    // === Setters === //
    void setRemainingSteps(int num_shells);

    // === Gameplay Function === //
    void advanceStepsClock();
    BoardSatelliteView TakeSatelliteImage(); // Returns an updated SatelliteView object at the start of the turn.
    std::map<int, ActionRequest> requestAlgorithmsActions();
    std::vector<bool> performActionsOnBoard(std::map<int, ActionRequest> actions, BoardSatelliteView &sat_view, GameCollisionHandler &c_handler, GameDrawer &d);
    void moveShells(int times, GameCollisionHandler &c_handler, GameDrawer &d);
    void moveShellsOnce(); // Move shells acoording to their direction
    bool concludeGame();   // Checks if a game is finsihed in a specific turn

    // === Log Functions === //
    void setOutputFile(const std::string &input_file_path);

    void logStepActions(const std::map<int, ActionRequest> &actions, std::vector<bool> is_valid_action) const;
    void logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const;
    void logKilled(bool coma) const;

    void logComa() const;
    void logEndOfStep() const;
    void logWin(int winner, int remaining_tanks) const;

    void logTie(const std::string &reason) const;
    void logZeroTanksTie() const;
    void logZeroShellsTie() const;
    void logMaxStepsTie() const;

    // === Prepare Run Functions === //
    void prepareForRun();
    void createPlayers();
    void createAlgorithms();

    // === Helper Functions === //
    void applyShellsLimitRuleOnRemainingSteps();

public:
    // === Constructor for Temp Factory Objects === //
    template <DerivedFromPlayerFactory PF, DerivedFromTankAlgorithmFactory AF>
    GameManager(PF &&player_factory, AF &&tank_algorithm_factory)
        : board(0, 0),
          player_factory(std::make_unique<std::decay_t<PF>>(std::forward<PF>(player_factory))),
          tank_algorithm_factory(std::make_unique<std::decay_t<AF>>(std::forward<AF>(tank_algorithm_factory))) {}

    // === Destructor === //
    ~GameManager() = default;

    // === Copy (Deleted) === //
    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;

    // === Move (Deleted) === //
    GameManager(GameManager &&) = delete;
    GameManager &operator=(GameManager &&) = delete;

    // === Public API === //
    // Reads input file into board object.
    bool readBoard(const std::string &input_file_path);

    // === Runs Game === //
    void run(DrawingType dt = DrawingType::NONE);
};
