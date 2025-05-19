#include "../common/Player.h"

template <int player_number>
class PlayerN : public Player
{
public:
    //=== Consturctor ===//
    PlayerN(size_t x, size_t y,
            size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells) {}

    //=== Destructor ===//
    ~PlayerN() override {}

    //=== Player Logic ===//
    void updateTankWithBattleInfo(TankAlgorithm &tank, SatelliteView &satellite_view) override
    {
        // Implement the logic for PlayerN's tank update here
        // For example:
        // tank.moveForward();
        // tank.fireShell();
    }
};
