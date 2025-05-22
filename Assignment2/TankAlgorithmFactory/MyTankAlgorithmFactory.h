#pragma once

#include <memory>
#include "../common/TankAlgorithmFactory.h"
#include "../TankAlgorithm/AggressiveTankAlgorithm.h"
#include "../TankAlgorithm/PassiveTankAlgorithm.h"

using std::unique_ptr;

// TODO: finish implementation
class MyTankAlgorithmFactory : public TankAlgorithmFactory{
    public:

        // === Destructor === //
        ~MyTankAlgorithmFactory() override = default;

        // === Producing TankAlgorithm objects === //
        unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override
        {
            if (player_index % 2 == 1)
                return std::make_unique<AggressiveTankAlgorithm>(player_index, tank_index);

            return std::make_unique<PassiveTankAlgorithm>(player_index, tank_index);
        }
};