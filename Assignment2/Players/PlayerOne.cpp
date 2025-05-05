#include "Player.h"

class PlayerOne : public Player{
public:

//=== Consturctor ===//
    PlayerOne(int player_index,
              size_t x, size_t y,
              size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells) {}

//=== Destructor ===//
    ~PlayerOne() override {}

    
//=== Player Logic ===// 
    void updateTankWithBattleInfo(TankAlgorithm &tank, SatelliteView &satellite_view) override
    {
        // Implement the logic for PlayerOne's tank update here
        // For example:
        // tank.moveForward();
        // tank.fireShell();
    }
};
