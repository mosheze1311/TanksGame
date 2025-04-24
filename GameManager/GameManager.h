#pragma once
#include <fstream>

#include "../GamePlayer/GamePlayer.h"
#include "../GameBoard/GameBoard.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameDrawer/GameDrawer.h"
#include <optional>

class GameManager
{
private:
    //=== Attributes ===
    GameBoard &board;
    Player p1, p2;
    int remaining_turns = 40;

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
    GameManager(GameBoard &board, Player p1, Player p2);

    //=== Functions ===
    void play(DrawingType dt);
};