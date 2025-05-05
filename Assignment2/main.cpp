#include "GameManager.h"
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"

class TempTankAlgorithm : public TankAlgorithm
{
    ActionRequest getAction() override
    {
        return ActionRequest::DoNothing;
    };

    void updateBattleInfo(BattleInfo &info) override {

    };
};
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
class TempTankAlgorithmFactory : public TankAlgorithmFactory{
    
     unique_ptr<TankAlgorithm> create(
        int player_index, int tank_index) const override
        {
            return std::make_unique<TempTankAlgorithm>();
        };
};

TempPlayerFactory getPlayerFactory()
{
    return TempPlayerFactory();
}
TempTankAlgorithmFactory getTankAlgorithmFactory()
{
    return TempTankAlgorithmFactory();
}

int main(int argc, char **argv)
{
    GameManager game(getPlayerFactory(), getTankAlgorithmFactory());
    game.readBoard("InputFiles/input1.txt");
    game.run();
}
    