#pragma once

#include "../common/Player.h"

#include "../GameBoard/GameBoard.h"
#include "../BattleInfo/BattleInfoAgent.h"

#include <map>
#include <vector>

#define analysis_max_steps_gap 5UL

class AbstractPlayer : public Player
{
protected:
    // === Attributes === //
    int player_index;

    size_t board_width;
    size_t board_height;
    size_t max_steps;
    size_t initial_num_of_shells;

    std::map<size_t, BoardCell> tanks_locations; // maps tank id to its location
    BoardCell caller_tank_location;
    size_t current_step = 1;
    size_t tanks_alive = 0;
    SatelliteAnalyticsView analytics_view;

    // === Pure Virtual Function === //
    virtual size_t calculateStepsGapForGetInfo() = 0;

    // === Helper Functions === //
    BoardCell parseSatelliteView(const SatelliteView &satellite_view);

    PlayerToTankDetails createPlayerToTankDetails(const SatelliteView &satellite_view);

public:
    // === Constructor === //
    AbstractPlayer(int player_index,
                   size_t x, size_t y,
                   size_t max_steps, size_t num_shells);

    // === Destructor === //
    ~AbstractPlayer() override = default;

    // === Player Interface === //    
    void updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view) override;
};