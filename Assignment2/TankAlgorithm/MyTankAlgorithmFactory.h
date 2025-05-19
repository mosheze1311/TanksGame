#pragma once

#include <memory>
#include "../common/TankAlgorithmFactory.h"
#include "../TankAlgorithm/MyTankAlgorithm.h"

using std::unique_ptr;

// TODO: finish implementation
class MyTankAlgorithmFactory : public TankAlgorithmFactory{
    public:

        // === Destructor === //
        ~MyTankAlgorithmFactory() override = default;

        // === Producing TankAlgorithm objects === //

        // Creates a new instance of MyTankAlgorithm
        unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override
        {
            return std::make_unique<AggressiveTankAlgorithm>(player_index, tank_index);
        }
};