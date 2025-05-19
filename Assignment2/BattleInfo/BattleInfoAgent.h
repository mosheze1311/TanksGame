#pragma once

#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"
#include "../GameBoard/GameBoard.h"
#include "../common/ActionRequest.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include <vector>
#include <queue>
#include <optional>


class BattleInfoAgent : public BattleInfo
{

private:
    // === Data sent from Player to TankAlgorithm === //
    // Player to tank (game static data)
    size_t max_steps;
    size_t board_rows;
    size_t board_cols;
    size_t max_num_of_shells;
    
    // Player to tank (dynamically change mid game)
    SatelliteAnalyitcsView &advanced_sat_view;
    BoardCell current_cell;
    size_t estimated_enemy_remaining_shells;
    size_t step_to_get_info;

    // Tank to player
    size_t &tank_index;
    size_t &current_step;
    size_t &remaining_shells;
    BoardCell &enemy_target_location;

public:
    // === Constructor === //
    BattleInfoAgent(SatelliteView &sat_view, BoardCell tank_location,
                    size_t max_steps, size_t shells, size_t rows, size_t cols, size_t &tank_idx, size_t &current_step, size_t &remaining_shells)
        : 
        // d
        advanced_sat_view(advanced_sat_view),
        current_cell(tank_location),

          // static data
          max_steps(max_steps),
          max_num_of_shells(shells),
          board_rows(rows),
          board_cols(cols),

          tank_index(tank_idx),
          current_step(current_step),
          remaining_shells(remaining_shells) {};

    // === Destructor === //
    ~BattleInfoAgent() override = default;

    // === Getters === //
    BoardCell getCurrentCell() const { return current_cell; }
    size_t getTankIndex() const { return tank_index; }
    size_t getMaxSteps() const { return max_steps; }
    size_t getCurrentStep() const { return current_step; }
    size_t getRemainingShells() const { return remaining_shells; }

    // === Setters === //

    void setRemainingShells(size_t shells)
    {
        remaining_shells = shells;
    }

    void setCurrentStep(size_t step)
    {
        current_step = step;
    }

    // === Additional methods (rename later) === //
    

    // TODO: Rethink about functions that can be added here.
};