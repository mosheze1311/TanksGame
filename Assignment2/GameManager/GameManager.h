#pragma  once
#include "../common/PlayerFactory.h"
#include "../common/TankAlgorithmFactory.h"

#include "../BoardFactory/BoardFactory.h"
#include "../GameDrawer/GameDrawer.h"
#include "../GameBoard/GameBoard.h"
#include "../SatelliteView/BoardSatelliteView.h"
#include "../Logger/Logger.h"

#include <fstream>
#include <sstream>
#include <string>
#include <set>


class GameManager{
private:
    //=== Attributes ===
    GameBoard board;
    size_t remaining_turns;

    const PlayerFactory& player_factory;
    const TankAlgorithmFactory& tank_algorithm_factory;

    map<Tank*, unique_ptr<TankAlgorithm>> tanks_algorithms;
     

    string output_file_name;

    //=== Getters ===
    int getRemainingTurns() const; // private getter for readability

    //=== Gameplay Function ===
    bool concludeGame(); // Checks if a game is finsihed in a specific turn
    void performActionsOnBoard(map<Tank *, ActionRequest> actions);
    void moveShells(); // Move shells acoording to their direction
    BoardSatelliteView TakeSatelliteImage(); // Returns an updated SatelliteView object at the start of the turn.

    //=== Log Functions===
    void logAction(Tank *tank, ActionRequest action, bool is_valid);
    void logWin(int winner, int remaining_tanks);
    void logZeroTanksTie();
    void logMaxStepsTie();
    void logTie(string reason);

public:    
    //=== Constructors ===
    GameManager(const PlayerFactory& player_factory, const TankAlgorithmFactory& tank_algorithm_factory);

    //=== Functions ===
    // Reads input file into board object. 
    bool readBoard(std::string input_file_path);

    // Runs the game
    void run(DrawingType dt = DrawingType::NONE);
};
