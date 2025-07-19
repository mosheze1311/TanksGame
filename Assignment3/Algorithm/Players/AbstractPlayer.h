#pragma once

#include "../../common/Player.h"

#include "../../UserCommon/GameBoard/GameBoard.h"
#include "../BattleInfo/BattleInfoAgent.h"
#include "../Tactics/Tactic.h"
#include "../Formation/Formation.h"

#include <map>
#include <vector>
#include <set>

namespace Algorithm_211388913_322330820
{
using namespace UserCommon_211388913_322330820;
#define analysis_max_steps_gap 3UL

    class AbstractPlayer : public Player
    {
    protected:
        // === Attributes === //
        int player_index;

        size_t board_width;
        size_t board_height;
        size_t max_steps;
        size_t initial_num_of_shells;

        BoardCell caller_tank_location;
        std::set<BoardCell> my_tanks_locations;
        size_t current_step = 1;
        SatelliteAnalyticsView analytics_view;

        std::map<size_t, TankDetails> tanks_details;
        std::set<Formation> formations;

        // === Pure Virtual Function === //
        virtual size_t calculateStepsGapForGetInfo() = 0;

        // === Helper Functions === //
        void parseSatelliteView(const SatelliteView &satellite_view);

        PlayerToTankDetails createPlayerToTankDetails();

        // === Helper Functions === //
        void removeDeadTanks(const std::set<size_t>& dead_tank_ids);
       std::set<size_t> getDeadTanksIds(SatelliteView &satellite_view) const;
    public:
        // === Constructor === //
        AbstractPlayer(int player_index,
                       size_t x, size_t y,
                       size_t max_steps, size_t num_shells);

        // === Destructor === //
        virtual ~AbstractPlayer() override = default;

        // === Player Interface === //
        void updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view) override;
    };
}