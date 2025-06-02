#pragma once

#include "../common/Player.h"
#include "../common/PlayerFactory.h"

#include "TestBattleInfo.h"

#include <vector>

class TestPlayer : public Player
{
private:
    std::vector<ActionRequest> action_requests;

public:
    TestPlayer(int player_index, size_t x, size_t y,
               size_t max_steps, size_t num_shells) : Player(player_index, x, y, max_steps, num_shells) {};
    ~TestPlayer() override = default;

    void updateTankWithBattleInfo(TankAlgorithm &t, SatelliteView &sat_view) override
    {
        (void)sat_view; // ignore pedantic errors

        TestBattleInfo battle_info;
        t.updateBattleInfo(battle_info);
    };
};

class TestPlayerFactory : public PlayerFactory
{
public:
    TestPlayerFactory() {};

    std::unique_ptr<Player> create(int player_index, size_t x, size_t y,
                                   size_t max_steps, size_t num_shells) const override
    {
        return std::make_unique<TestPlayer>(player_index, x, y, max_steps, num_shells);
    }
};
