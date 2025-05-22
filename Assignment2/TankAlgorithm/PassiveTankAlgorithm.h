#pragma once

#include "../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

class PassiveTankAlgorithm : public AbstractTankAlgorithm
{
public:
    // === Constructor === //
    PassiveTankAlgorithm(size_t player_index, size_t tank_index);

    // === Destructor === //
    ~PassiveTankAlgorithm() override;

    // === Public Interface === //
    ActionRequest getAction() override;

    void updateBattleInfo(BattleInfo &info) override;
};