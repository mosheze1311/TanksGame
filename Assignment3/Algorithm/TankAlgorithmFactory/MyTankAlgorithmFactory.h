#pragma once

#include "../../common/TankAlgorithm.h"
#include "../TankAlgorithm/AggressiveTankAlgorithm.h"
#include "../TankAlgorithm/PassiveTankAlgorithm.h"

#include <memory>
namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    TankAlgorithmFactory myAlgorithmFactory = [](int player_index, int tank_index) -> std::unique_ptr<TankAlgorithm>
    {
        if ((tank_index + player_index) % 2 == 1)
            return std::make_unique<AggressiveTankAlgorithm>(player_index, tank_index);

        return std::make_unique<PassiveTankAlgorithm>(player_index, tank_index);
    };
}