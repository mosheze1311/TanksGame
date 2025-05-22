#pragma once

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../GameBoardUtils/GameBoardUtils.h"

class TankAlgorithmUtils
{
private:
    TankAlgorithmUtils(/* args */) = delete;
    ~TankAlgorithmUtils() = delete;

public:
    static bool isShellChasingTank(BoardCell shell_loc, AssumedDirection shell_assumed_dir ,BoardCell tank_loc, size_t width, size_t height);
    
    static bool inShootRange(BoardCell from, BoardCell to, size_t width, size_t height);
};