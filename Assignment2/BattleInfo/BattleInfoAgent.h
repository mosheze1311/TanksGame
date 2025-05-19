#pragma once

#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"
#include "../GameBoard/GameBoard.h"
#include "../common/ActionRequest.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include <vector>
#include <queue>
#include <optional>

struct GameDetails
{
    size_t max_steps;
    size_t board_rows;
    size_t board_cols;
    size_t max_num_of_shells;
};

struct TankToPlayerDetails
{
    size_t tank_index;
    size_t current_step;
    size_t remaining_shells;
    BoardCell enemy_target_location;
};

struct PlayerToTankDetails
{
    BoardCell current_cell;
    size_t estimated_enemy_remaining_shells;
    size_t step_to_get_info;
};

class BattleInfoAgent : public BattleInfo
{

private:
    GameDetails game_details;
    SatelliteAnalyitcsView &advanced_sat_view;

    PlayerToTankDetails player_to_tank;
    TankToPlayerDetails &tank_to_player;

public:
    // === Constructor === //
    BattleInfoAgent(SatelliteAnalyitcsView &advanced_sat_view, PlayerToTankDetails player_to_tank, GameDetails details, TankToPlayerDetails &tank_to_player)
        : // dynamic data to tank
          advanced_sat_view(advanced_sat_view),
          player_to_tank(player_to_tank),

          // static data
          game_details(details),

          // dynamic data from tank
          tank_to_player(tank_to_player) {};

    // === Destructor === //
    ~BattleInfoAgent() override = default;

// === Getters === //
BoardCell getCurrentCell() const { return player_to_tank.current_cell ; }
BoardCell getEnemyTargetLocation() const { return tank_to_player.enemy_target_location; }
size_t getEstimatedEnemyRemainingShells() const { return player_to_tank.estimated_enemy_remaining_shells; }
size_t getTankIndex() const { return tank_to_player.tank_index; }
size_t getMaxSteps() const { return game_details.max_steps; }
size_t getCurrentStep() const { return tank_to_player.current_step; }
size_t getRemainingShells() const { return tank_to_player.remaining_shells; }

// === Setters === //

void setCurrentCell(BoardCell cell)
{
    player_to_tank.current_cell = cell;
}

void setEnemyTargetLocation(BoardCell target)
{
    tank_to_player.enemy_target_location = target;
}

void setEstimatedEnemyRemainingShells(size_t shells)
{
    player_to_tank.estimated_enemy_remaining_shells = shells;
}

void setCurrentStep(size_t step)
{
    tank_to_player.current_step = step;
}

void setRemainingShells(size_t shells)
{
    tank_to_player.remaining_shells = shells;
}

    // === INIT Analytics === //
};