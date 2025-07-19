#pragma once

#include "../../common/ActionRequest.h"
#include "../../common/BattleInfo.h"
#include "../../common/SatelliteView.h"

#include "../../UserCommon/GameBoard/GameBoard.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include "../Tactics/Tactic.h"
#include "../Formation/Formation.h"

#include <optional>
#include <queue>
#include <vector>
namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;

    // === Container Structs === //
    struct GameDetails
    {
        size_t max_steps;
        size_t board_rows;
        size_t board_cols;
        size_t initial_num_of_shells;

        GameDetails(size_t max_steps, size_t board_rows, size_t board_cols, size_t initial_num_of_shells)
            : max_steps(max_steps), board_rows(board_rows), board_cols(board_cols), initial_num_of_shells(initial_num_of_shells) {};
    };

    struct TankToPlayerDetails
    {
        size_t tank_index;
        size_t current_step;
        size_t remaining_shells;
        Direction dir;
        std::unique_ptr<Tactic> tactic;

        // no need for constructor - updated by algorithm only after init
    };

    struct PlayerToTankDetails
    {
        BoardCell caller_tank_location;
        size_t steps_gap_for_get_info;
        std::set<Formation> formations;

        PlayerToTankDetails(const BoardCell &caller_tank_location,
                            size_t steps_gap_for_get_info,
                            const std::set<Formation> &formations)
            : caller_tank_location(caller_tank_location),
              steps_gap_for_get_info(steps_gap_for_get_info),
              formations(formations) {}
    };

    // === Class Declaration === //
    class BattleInfoAgent : public BattleInfo
    {
    private:
        // === Attributes === //
        const GameDetails game_details;

        SatelliteAnalyticsView &advanced_sat_view;
        const SatelliteView &new_satellite_image;

        PlayerToTankDetails player_to_tank;
        TankToPlayerDetails tank_to_player;

        // === Two-Steps Update View API === //
        void updateViewForStep(size_t steps_gap);

    public:
        // === Constructor === //
        BattleInfoAgent(SatelliteAnalyticsView &advanced_sat_view, const SatelliteView &sat_view, PlayerToTankDetails &player_to_tank, const GameDetails &details);

        // === Getters (for Player) === //
        size_t getCurrentStep() const;
        size_t getTankIndex() const;
        Direction getTankDirection() const;
        size_t getRemainingShells() const;

        // === Getters (for TankAlgorithm) === //
        size_t getMaxSteps() const;
        size_t getInitialNumShells() const;
        size_t getBoardWidth() const;
        size_t getBoardHeight() const;
        BoardCell getTankLocation() const;
        size_t getStepToGetInfo(size_t current_step) const;
        Formation getFormation() const;
        std::unique_ptr<Tactic> getTactic() const;

        // === Setters (for TankAlgorithm) === //
        void setCurrentStep(size_t step);
        void setTankDirection(Direction dir);
        void setRemainingShells(size_t shells);
        void setTactic(std::unique_ptr<Tactic> new_tactic);

        // === Two-Steps Update View API === //
        SatelliteAnalyticsView updateAndGetAnalyticsView(size_t current_step, const SatelliteAnalyticsView &tank_algorithm_sat_view);

        

    };
}