#pragma once

#include "../common/ActionRequest.h"
#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"

#include "../GameBoard/GameBoard.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"

#include <optional>
#include <queue>
#include <vector>

// === Container Structs ===
struct GameDetails
{
    size_t max_steps;
    size_t board_rows;
    size_t board_cols;
    size_t max_num_of_shells;
};

struct TankToPlayerDetails
{
    size_t tank_index = 0;
    size_t current_step = 0;
    size_t remaining_shells = 0;
    BoardCell enemy_target_location = BoardCell(0,0);
    Direction dir = Direction::RIGHT; 
};

struct PlayerToTankDetails
{
    BoardCell current_cell;
    size_t estimated_enemy_remaining_shells;
    size_t step_to_get_info;
};

// === Class Declaration ===
class BattleInfoAgent : public BattleInfo
{
private:
    // === Attributes ===
    GameDetails game_details;
    SatelliteAnalyitcsView &advanced_sat_view;

    PlayerToTankDetails player_to_tank;
    TankToPlayerDetails tank_to_player;

public:
    // === Constructor ===
    BattleInfoAgent(SatelliteAnalyitcsView &advanced_sat_view, PlayerToTankDetails player_to_tank, GameDetails details);

    // === Destructor ===
    ~BattleInfoAgent() override;

    // === Getters ===
    BoardCell getCurrentCell() const;
    BoardCell getEnemyTargetLocation() const;
    size_t getEstimatedEnemyRemainingShells() const;
    size_t getTankIndex() const;
    size_t getMaxSteps() const;
    size_t getCurrentStep() const;
    size_t getRemainingShells() const;
    Direction getTankDirection() const;

    // === Setters ===
    void setCurrentCell(const BoardCell& cell);
    void setEnemyTargetLocation(const BoardCell& target);
    void setEstimatedEnemyRemainingShells(size_t shells);
    void setCurrentStep(size_t step);
    void setRemainingShells(size_t shells);
    void setTankDirection(Direction dir);
};