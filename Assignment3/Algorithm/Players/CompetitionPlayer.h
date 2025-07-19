#pragma once

#include "../../common/Player.h"

#include "AbstractPlayer.h"

namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;

    class CompetitionPlayer : public AbstractPlayer
    {
        protected:
        size_t steps_gap = 0;

        size_t calculateStepsGapForGetInfo() override
        {
            if (this->current_step == 1)
            {

                return ++steps_gap + analysis_max_steps_gap;
            }
            else
            {
                steps_gap = this->my_tanks_locations.size();
                return std::max(steps_gap, analysis_max_steps_gap);
            }
        }

        public:
         // === Consturctor === //
        CompetitionPlayer(int player_index,
                size_t x, size_t y,
                size_t max_steps, size_t num_shells)
            : AbstractPlayer(player_index, x, y, max_steps, num_shells) {}

        // === Destructor === //
        ~CompetitionPlayer() override {}
    };
}
