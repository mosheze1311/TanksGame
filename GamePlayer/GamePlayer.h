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
    //===Attributes===
    const GameObjectType tanks_type;

    //===Functions===
    GameObjectType getEnemyTanksType();

    TankAction getTankAction(const GameBoard &board, const Tank *tank);
    TankAction getEvasionAction(const GameBoard &board, const Tank *tank, const MovableObject *evade_from);
    TankAction getAggressiveAction(const GameBoard &board, const Tank *tank);

    void Dijkstra(const GameBoard &board, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents);
    TankAction advanceTankToTarget(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target);
    bool isShellChasingTank(const GameBoard &board, const Tank *tank, const Shell *shell);
    bool canEnemyKillTank(const GameBoard &board, const Tank *tank, const Tank *enemy);
    BoardCell approxClosestEnemyTankLocation(const GameBoard &board, BoardCell start);
    TankAction adjustDirection(const GameBoard&board,BoardCell from, BoardCell to, Direction dir);
    
    //===helpers===
    bool inShootRange(const GameBoard &board, BoardCell from, BoardCell to);
    bool isDirectionMatch(BoardCell from, BoardCell to, Direction dir) const;
    bool canAdjustDirection(BoardCell from, BoardCell to) const;
    BoardCell getAdjustToCellTowardsTarget(BoardCell from, BoardCell to) const;
    optional<TankAction> escapeShells(const GameBoard &board, const Tank *tank);
    vector<BoardCell> getAdjacentCells(const GameBoard &board, BoardCell curr_cell) const;
    optional<BoardCell> getEscapingRoute(const GameBoard &board ,const Tank *tank, BoardCell current_cell, Direction enemy_dir) const;
    //===Algorithms===
    TankAction player1Algorithm(const GameBoard &board, const Tank *tank);
    TankAction player2Algorithm(const GameBoard &board, const Tank *tank);

public:
    Player(GameObjectType tanks_type);
    map<Tank *, TankAction> getActionsFromPlayer(const GameBoard &board);
};
