#pragma once
#include "../common/PlayerFactory.h"
#include "../common/TankAlgorithmFactory.h"

#include "../GameBoardInitializer/GameBoardInitializer.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameDrawer/GameDrawer.h"
#include "../GameBoard/GameBoard.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../Logger/Logger.h"

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

    const PlayerFactory &player_factory;
    const TankAlgorithmFactory &tank_algorithm_factory;

    vector<Tank *> tanks;
    vector<unique_ptr<TankAlgorithm>> algorithms;
    map<int, unique_ptr<Player>> players_map;

    string output_file_name;

    //=== Getters ===
    size_t getRemainingSteps() const; // private getter for readability
    vector<GameObjectType> getActiveTankTypes(map<GameObjectType, size_t> players_tanks_count) const;

    //=== Setters ===
    void setRemainingSteps(int nnum_shells);

    //=== Gameplay Function ===
    bool concludeGame(); // Checks if a game is finsihed in a specific turn
    void performActionsOnBoard(map<int, ActionRequest> actions, SatelliteView& sat_view);
    void moveShells(int times, GameCollisionHandler &c_handler, GameDrawer &d);
    void moveShellsOnce();                   // Move shells acoording to their direction
    BoardSatelliteView TakeSatelliteImage(); // Returns an updated SatelliteView object at the start of the turn.
    map<int, ActionRequest> requestAlgorithmsActions();
    void advanceStepsClock();

    //=== Log Functions===
    void setOutputFile(std::string &input_file_path);

    void logAction(ActionRequest action, bool is_valid, bool is_killed, bool coma) const;
    void logKilled(bool coma) const;

    void logComa() const;
    void logEndOfStep() const;
    void logWin(int winner, int remaining_tanks) const;

    void logTie(string reason) const;
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
    GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory);

    //=== Functions ===
    // Reads input file into board object.
    bool readBoard(std::string input_file_path);

    // Runs the game
    void run(DrawingType dt = DrawingType::NONE);
    };
