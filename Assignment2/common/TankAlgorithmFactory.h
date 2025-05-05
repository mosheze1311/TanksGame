#pragma once
#include <memory>
#include "TankAlgorithm.h"

using namespace std;

class TankAlgorithmFactory
{
public:
    virtual ~TankAlgorithmFactory() {}
    virtual unique_ptr<TankAlgorithm> create(
        int player_index, int tank_index) const = 0;
};
