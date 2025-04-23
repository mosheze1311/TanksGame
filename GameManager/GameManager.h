#pragma once
#include <fstream>

#include "../GamePlayer/GamePlayer.h"
#include "../GameBoard/GameBoard.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameDrawer/GameDrawer.h"
#include <optional>


class GameManager{
private:
    GameBoard& board;
    Player p1;
    Player p2;

    int remaining_turns = 40;
    ofstream output_file;

    void performPlayerActionsOnBoard(map<Tank *, TankAction> actions);
    // bool isUserActionValid(pair<Tank *, TankAction> action_pair) const;

    void logAction(Tank* tank, TankAction action, bool is_valid);

    
    
public : GameManager(GameBoard& board, Player p1, Player p2);
    void play(DrawingType dt);
    void simulateGame();

    //     // should call p1, p2 .getActions() and perform actions on the board until someone wins or there is a tie
    //     // output actions to the output file in some format    

    bool concludeGame(); // Checks if a game is finsihed in a specific turn
    int getRemainingTurns() const; 
    void logWin(bool is_player1_winner, string reason); // Logs a win if happens
    void logTie(string reason); // Logs a tie if happens

    void moveShells(GameCollisionHandler& c_handler, GameDrawer d); // Move shells acoording to their direction
};