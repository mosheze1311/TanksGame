#include "GameBoard/GameBoard.h"
#include "GameManager/GameManager.h"
#include "BoardFactory/BoardFactory.h"
#include "GamePlayer/GamePlayer.h"
#include "Logger/Logger.h"

int main(int argc, char *argv[])
{
    if (argc != 2){
        Logger::runtime().logError(std::format("Arguments do not match to programm - expecting 1, got {}", argc-1));
        return 0;
    }
    const string file_path = argv[1];
    BoardFactory factory;
    GameBoard b = factory.createGameBoard(file_path);
    Player p1;
    Player p2;

    GameManager gm(b, p1, p2,(string) "");
    gm.play();
}