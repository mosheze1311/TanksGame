#include "GameManager/GameManager.h"
#include "PlayerFactory/MyPlayerFactory.h"
#include "TankAlgorithmFactory/MyTankAlgorithmFactory.h"

#include <memory>
// TODO: maybe change to unique?
std::shared_ptr<PlayerFactory> getPlayerFactory()
{
    return std::make_shared<MyPlayerFactory>();
}

std::shared_ptr<TankAlgorithmFactory> getTankAlgorithmFactory()
{
    return std::make_shared<MyTankAlgorithmFactory>();
}

bool validateArgc(int argc)
{
    if (argc != 2 && argc != 3)
    {
        Logger::runtime().log(std::format("Arguments do not match to programm - expecting 1 (or 2 for drawing), got {}", argc - 1));
        return false;
    }

    return true;
}
bool validateDrawingType (DrawingType& dt, int argc, char** argv){
    if (argc == 3) // if drawing is requested
    {
        try
        {
            dt = DrawingTypes::fromInt(std::stoi(argv[2]));
        }
        catch (...)
        {
            Logger::runtime().log(std::format("If given, 3rd argument must be a number"));
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv)
{
    if(!validateArgc(argc))
        return EXIT_SUCCESS;

    const std::string file_path = argv[1];

    DrawingType dt = DrawingType::NONE; // default
    if (!validateDrawingType(dt, argc, argv))
        return EXIT_SUCCESS;

    GameManager game(getPlayerFactory(), getTankAlgorithmFactory());

    if(!game.readBoard(file_path))
        return EXIT_SUCCESS;

    game.run(dt);
    return EXIT_SUCCESS;
}
