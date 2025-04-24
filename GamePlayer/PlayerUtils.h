#pragma once

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

class PlayerUtils {
private:
    PlayerUtils(/* args */) = delete;
    ~PlayerUtils() = delete;

public:
    static void Dijkstra(const GameBoard &board, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents);
    static bool isShellChasingTank(const GameBoard &board, const Tank *tank, const Shell *shell);
    
    static bool inShootRange(const GameBoard &board, BoardCell from, BoardCell to);
    static BoardCell getNextCellInStraightLine(BoardCell from, BoardCell to);
};