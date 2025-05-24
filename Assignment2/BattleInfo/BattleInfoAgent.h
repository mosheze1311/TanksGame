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

    GameDetails(size_t max_steps, size_t board_rows, size_t board_cols, size_t max_num_of_shells)
        : max_steps(max_steps), board_rows(board_rows), board_cols(board_cols), max_num_of_shells(max_num_of_shells){};
};

struct TankToPlayerDetails
{
    size_t tank_index = 0;
    size_t current_step = 0;
    size_t remaining_shells = 0;
    BoardCell enemy_target_location = BoardCell(0, 0);
    Direction dir = Direction::RIGHT;

    TankToPlayerDetails(size_t tank_index = 0,
                        size_t current_step = 0,
                        size_t remaining_shells = 0,
                        BoardCell enemy_target_location = BoardCell(0, 0),
                        Direction dir = Direction::RIGHT)
        : tank_index(tank_index),
          current_step(current_step),
          remaining_shells(remaining_shells),
          enemy_target_location(enemy_target_location),
          dir(dir) {}
};

struct PlayerToTankDetails
{
    BoardCell current_cell;
    size_t estimated_enemy_remaining_shells;
    size_t step_to_get_info;

    PlayerToTankDetails(const BoardCell &current_cell = BoardCell(0, 0),
                        size_t estimated_enemy_remaining_shells = 0,
                        size_t step_to_get_info = 0)
        : current_cell(current_cell),
          estimated_enemy_remaining_shells(estimated_enemy_remaining_shells),
          step_to_get_info(step_to_get_info) {}
};

// === Class Declaration === //
class BattleInfoAgent : public BattleInfo
{
private:
    // === Attributes === //
    GameDetails game_details;
    SatelliteAnalyitcsView &advanced_sat_view;

    PlayerToTankDetails player_to_tank;
    TankToPlayerDetails tank_to_player;
    SatelliteView &new_satellite_image;

public:
    // === Constructor === //
    BattleInfoAgent(SatelliteAnalyitcsView &advanced_sat_view, SatelliteView &sat_view, PlayerToTankDetails player_to_tank, GameDetails details);

    // === Destructor === //
    ~BattleInfoAgent() override;

    // === Getters === //
    BoardCell getCurrentCell() const;
    BoardCell getEnemyTargetLocation() const;
    size_t getEstimatedEnemyRemainingShells() const;
    size_t getTankIndex() const;
    size_t getMaxSteps() const;
    size_t getCurrentStep() const;
    size_t getRemainingShells() const;
    Direction getTankDirection() const;
    size_t getMaxShells() const;
    SatelliteAnalyitcsView getAnalyticsView() const;

    // === Setters === //
    void setCurrentCell(const BoardCell &cell);
    void setEnemyTargetLocation(const BoardCell &target);
    void setEstimatedEnemyRemainingShells(size_t shells);
    void setCurrentStep(size_t step);
    void setRemainingShells(size_t shells);
    void setTankDirection(Direction dir);

    // === Two-Steps Update View API === //
    // TODO: move this logic to battle info boject so that SatelliteAnalyitcsView wont save SatelliteView refernce as an attribute
    void updateViewForStep(size_t steps_gap);
};