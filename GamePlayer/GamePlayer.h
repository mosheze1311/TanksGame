#pragma once
#include <vector>
#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/TankAction.h"

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

    void Dijkstra(const GameBoard &board, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents); // Sholud have logic for Dijkstra search
    TankAction advanceTankToTarget(const GameBoard &board, const Tank *tank, BoardCell start, BoardCell target);
    bool isShellChasingTank(const GameBoard &board, const Tank *tank, const Shell *shell);
    bool canEnemyKillTank(const GameBoard &board, const Tank *tank, const Tank *enemy);
    BoardCell approxClosestEnemyTankLocation(const GameBoard &board, BoardCell start);
    TankAction adjustDirection(const GameBoard&board,BoardCell from, BoardCell to, Direction dir);
    
    //===helpers===
    bool inShootRange(BoardCell from, BoardCell to);
    bool isDirectionMatch(BoardCell from, BoardCell to, Direction dir) ;
    bool canAdjustDirection(BoardCell from, BoardCell to) ;
    public : Player(GameObjectType tanks_type);
    map<Tank *, TankAction> getActionsFromPlayer(const GameBoard &board);
};
