#pragma once

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

class PlayerUtils {
public:
    static void Dijkstra(const GameBoard &board, GameObjectType tank_type, BoardCell start, BoardCell target, map<BoardCell, int> &distances, map<BoardCell, BoardCell> &parents);

};