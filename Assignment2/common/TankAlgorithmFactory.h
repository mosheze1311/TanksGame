#pragma once
#include "TankAlgorithm.h"
#include <memory>

class TankAlgorithmFactory
{
public:
    virtual ~TankAlgorithmFactory() {};
    virtual std::unique_ptr<TankAlgorithm> create(
        int player_index, int tank_index) const = 0;
};
