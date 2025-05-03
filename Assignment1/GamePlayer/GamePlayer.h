#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/TankAction.h"
#include <optional>

using namespace std;

class Player
{

private:
    //=== Attributes ===
    const GameObjectType tanks_type;

    //=== Getters ===
    GameObjectType getEnemyTanksType() const;

    //=== Core Logic Functions ===
    TankAction decideTankAction(const GameBoard &board, const Tank *tank) const;
    TankAction getTankEvasionAction(const GameBoard &board, const Tank *tank, const MovableObject *evade_from) const;
    TankAction getTankAggressiveAction(const GameBoard &board, const Tank *tank) const;

    TankAction advanceTankToTarget(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target) const;
    TankAction adjustDirection(const GameBoard &board, BoardCell from, BoardCell to, Direction dir) const;

    //=== Helper Functions ===
    BoardCell approxClosestEnemyTankLocation(const GameBoard &board, BoardCell start) const;
    optional<TankAction> escapeShells(const GameBoard &board, const Tank *tank) const;
    optional<BoardCell> getEscapingRoute(const GameBoard &board ,const Tank *tank, BoardCell current_cell, Direction enemy_dir) const;
    optional<TankAction> attemptShoot(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target) const;

    //=== Algorithm Implementations ===
    TankAction player1Algorithm(const GameBoard &board, const Tank *tank) const;
    TankAction player2Algorithm(const GameBoard &board, const Tank *tank) const;

public:
    //=== Constructor ===
    Player(GameObjectType tanks_type);
    
    //=== Interface For Manager Use ===
    map<Tank *, TankAction> getActionsFromPlayer(const GameBoard &board) const;
};
