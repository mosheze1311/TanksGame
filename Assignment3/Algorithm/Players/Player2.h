#pragma once

#include "../common/Player.h"

#include "AbstractPlayer.h"

#include <algorithm>

class Player2 : public AbstractPlayer
{

protected:
    size_t steps_gap = 0;

    size_t calculateStepsGapForGetInfo() override
    {
        if (this->current_step == 1)
        {

            return ++steps_gap;
        }
        else
        {
            steps_gap = this->tanks_alive;
            return std::max(tanks_alive, analysis_max_steps_gap);
        }
    }

public:
    // === Consturctor === //
    Player2(int player_index,
            size_t x, size_t y,
            size_t max_steps, size_t num_shells)
        : AbstractPlayer(player_index, x, y, max_steps, num_shells) {}

    // === Destructor === //
    ~Player2() override {}
};
