#pragma once

#include "../../common/Player.h"

#include "../Players/Player1.h"
#include "../Players/Player2.h"

#include <memory>

namespace Algorithm_211388913_322330820 
{
    using namespace UserCommon_211388913_322330820;
    PlayerFactory myPlayerFactory = [](int player_index, size_t x, size_t y, size_t max_steps, size_t num_shells) -> std::unique_ptr<Player>
    {
        return std::make_unique<Player1>(player_index, x, y, max_steps, num_shells);
    };
}
