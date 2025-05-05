#pragma  once
#include "../common/PlayerFactory.h"
#include "../common/TankAlgorithmFactory.h"

#include "../BoardFactory/BoardFactory.h"
#include "../GameDrawer/GameDrawer.h"
#include "../GameBoard/GameBoard.h"
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

    string output_file_name;

    //=== Getters ===
    int getRemainingTurns() const; // private getter for readability

    //=== Gameplay Function ===
    bool concludeGame(); // Checks if a game is finsihed in a specific turn
    void performPlayerActionsOnBoard(map<Tank *, TankAction> actions);
    void moveShells(); // Move shells acoording to their direction

    //=== Log Functions===
    void logAction(Tank *tank, TankAction action, bool is_valid);
    void logWin(bool is_player1_winner, string reason);
    void logTie(string reason);

public:    
    //=== Constructors ===
    GameManager(const PlayerFactory& player_factory, const TankAlgorithmFactory& tank_algorithm_factory);

    //=== Functions ===
    bool readBoard(std::string input_file_path);
    void run(DrawingType dt = DrawingType::NONE);
};
