#include "../common/Player.h"

class PlayerTwo : public Player{
public:

//=== Consturctor ===//
    PlayerTwo(int player_index,
              size_t x, size_t y,
              size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells) {}

//=== Destructor ===//
    ~PlayerTwo() override {}


    //=== Player Logic ===// 
    void updateTankWithBattleInfo(TankAlgorithm &tank, SatelliteView &satellite_view) override
    {
        // Implement the logic for PlayerTwo's tank update here
        // For example:
        // tank.moveForward();
        // tank.fireShell();
    }
};
