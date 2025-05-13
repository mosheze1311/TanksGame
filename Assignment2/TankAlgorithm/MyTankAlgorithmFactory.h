#pragma once

#include <memory>
#include "../common/TankAlgorithmFactory.h"

using std::unique_ptr;

// TODO: finish implementation
class MyTankAlgorithmFactory : public TankAlgorithmFactory{
    public:
        ~MyTankAlgorithmFactory() {};
        unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override;
};