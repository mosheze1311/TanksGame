#include "GameBoard/GameBoard.h"
#include "GameManager/GameManager.h"
#include "BoardFactory/BoardFactory.h"
#include "GamePlayer/GamePlayer.h"
#include "Logger/Logger.h"

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 3){
        Logger::runtime().logError(std::format("Arguments do not match to programm - expecting 1 (or 2 for drawing), got {}", argc-1));
        return 0;
    }
    const string file_path = argv[1];
    GameBoard* b = BoardFactory::createGameBoard(file_path);
    if(!b){
        Logger::runtime().logError(std::format("Can't create board"));
        return 0;
    }
    DrawingType dt = DrawingType::NONE;
    if(argc == 3) // if drawing is requested
    {
        try
        {
            dt = DrawingTypes::fromInt(std::stoi(argv[2]));
        }
        catch (...)
        {
            Logger::runtime().logError(std::format("If given, 3rd argument must be a number"));
            return 0;
        }
    }
    Player p1(GameObjectType::tank1), p2(GameObjectType::tank2);
    GameManager gm(*b, p1, p2);
    gm.play(dt);
    return 0;
}