#pragma once
#include "../common/PlayerFactory.h"
#include "../common/TankAlgorithmFactory.h"

#include "../GameBoardInitializer/GameBoardInitializer.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameDrawer/GameDrawer.h"
#include "../GameBoard/GameBoard.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../Logger/Logger.h"

#include <filesystem> // for splitting the file_path to name only
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include <vector>

class GameManager
{
private:
    //=== Attributes ===
    GameBoard board;

    size_t remaining_steps;
    bool are_steps_limited_by_shells = false;

    const std::shared_ptr<PlayerFactory> player_factory;
    const std::shared_ptr<TankAlgorithmFactory> tank_algorithm_factory;

    std::vector<Tank *> game_total_tanks;
    std::vector<std::unique_ptr<TankAlgorithm>> algorithms;
    std::map<int, std::unique_ptr<Player>> players_map;

    std::string output_file_name;

    //=== Getters ===
    size_t getRemainingSteps() const; // private getter for readability
    std::vector<GameObjectType> getActiveTankTypes(map<GameObjectType, size_t> players_tanks_count) const;

    //=== Setters ===
    void setRemainingSteps(int nnum_shells);

    //=== Gameplay Function ===
    void advanceStepsClock();
    BoardSatelliteView TakeSatelliteImage(); // Returns an updated SatelliteView object at the start of the turn.
    map<int, ActionRequest> requestAlgorithmsActions();
    std::vector<bool> performActionsOnBoard(map<int, ActionRequest> actions, BoardSatelliteView &sat_view, GameCollisionHandler &c_handler, GameDrawer &d);
    void moveShells(int times, GameCollisionHandler &c_handler, GameDrawer &d);
    void moveShellsOnce();                   // Move shells acoording to their direction
    bool concludeGame(); // Checks if a game is finsihed in a specific turn

    //=== Log Functions===
    void setOutputFile(std::string &input_file_path);

    void logStepActions(std::map<int, ActionRequest> actions, std::vector<bool> is_valid_action) const;
    void logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const;
    void logKilled(bool coma) const;

    void logComa() const;
    void logEndOfStep() const;
    void logWin(int winner, int remaining_tanks) const;

    void logTie(std::string reason) const;
    void logZeroTanksTie() const;
    void logZeroShellsTie() const;
    void logMaxStepsTie() const;

    //=== Prepare Run Functions ===
    void prepareForRun();
    void createPlayers();
    void createAlgorithms();

    //=== Helper Functions ===
    void applyShellsLimitRuleOnRemainingSteps();

public:
    //=== Constructors ===
    GameManager(const std::shared_ptr<PlayerFactory> player_factory, const std::shared_ptr<TankAlgorithmFactory> tank_algorithm_factory);

    //=== Public API ===
    // Reads input file into board object.
    bool readBoard(std::string input_file_path);

    // Runs the game
    void run(DrawingType dt = DrawingType::NONE);
};
