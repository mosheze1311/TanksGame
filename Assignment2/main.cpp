#include "GameManager/GameManager.h"
#include "common/PlayerFactory.h"
#include "TankAlgorithmFactory/MyTankAlgorithmFactory.h"

class TempPlayer : public Player{
public:
    TempPlayer(): Player(1,1,1,1,1){

    }
    void updateTankWithBattleInfo(TankAlgorithm &tank, SatelliteView &satellite_view) override{

    }
};
class TempPlayerFactory : public PlayerFactory{
    unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                      size_t max_steps, size_t num_shells)const override{
        return std::make_unique<TempPlayer>();
                                      };
};


TempPlayerFactory getPlayerFactory()
{
    return TempPlayerFactory();
}

MyTankAlgorithmFactory getTankAlgorithmFactory()
{
    return MyTankAlgorithmFactory();
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
            return 0;
        }
    }
}

int main(int argc, char **argv)
{
    if(!validateArgc(argc))
        return 0;

    const string file_path = argv[1];

    DrawingType dt = DrawingType::NONE; // default
    if (!validateDrawingType(dt, argc, argv))
        return 0;
    
    GameManager game(getPlayerFactory(), getTankAlgorithmFactory());

    if(!game.readBoard(file_path))
        return 0;

    game.run(dt);
}
