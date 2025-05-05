#pragma  once
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"
#include "GameBoard/GameBoard.h"
#include "Logger/Logger.h"
#include "BoardFactory/BoardFactory.h"
#include "GameDrawer/GameDrawer.h"

#include <fstream>
#include <sstream>
#include <string>
#include <set>
/*
 TODO: I added the 'createGameObjectOfType' method (that was originally on the Board Factory)
        It is using 'new' and should not remain - only here to allow testing for the 'readBoard' method.
*/ 

class GameManager{
private:
    //=== Attributes ===
    GameBoard board;
    size_t max_steps;
    size_t tanks_num_shells;

    const PlayerFactory& player_factory;
    const TankAlgorithmFactory& tank_algorithm_factory;
    

public:
    //~GameManager();
    GameManager(const PlayerFactory& player_factory, const TankAlgorithmFactory& tank_algorithm_factory);
    bool readBoard(std::string input_file_path);
    void run();
};

GameManager::GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

void GameManager::run(){
    //TODO: add this function
}

bool GameManager::readBoard(std::string input_file_path)
{
    return BoardFactory::initGameBoardFromFile(this->board, input_file_path);
}


// For later use - check if player has tanks before starting the game. Use in run() function.
// bool checkPreGameTanks(const Player player)
// {
//     return player.getTanks().size() > 0;
// }