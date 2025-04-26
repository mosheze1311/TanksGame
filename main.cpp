
#include "GameBoard/GameBoard.h"
#include "GameManager/GameManager.h"
#include "BoardFactory/BoardFactory.h"
#include "GamePlayer/GamePlayer.h"
#include "Logger/Logger.h"
#include <format>
#include <filesystem> // for splitting the file_path to name only
#include <optional>

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
    std::string input_file_name = std::filesystem::path(file_path).stem().string(); // this is a standard library function the return the name of a file
    Player p1(GameObjectType::TANK1), p2(GameObjectType::TANK2);
    GameManager gm(*b, p1, p2, "output_" + input_file_name + ".txt");
    gm.play(dt);
    delete b;
    
    return 0;
}