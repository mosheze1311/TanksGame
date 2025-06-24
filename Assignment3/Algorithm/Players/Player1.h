#pragma once

#include "../../common/Player.h"

#include "AbstractPlayer.h"
namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    class Player1 : public AbstractPlayer
    {

    protected:
        size_t calculateStepsGapForGetInfo() override
        {
            return analysis_max_steps_gap;
        }

    public:
        // === Consturctor === //
        Player1(int player_index,
                size_t x, size_t y,
                size_t max_steps, size_t num_shells)
            : AbstractPlayer(player_index, x, y, max_steps, num_shells) {}

        // === Destructor === //
        ~Player1() override {}
    };
}