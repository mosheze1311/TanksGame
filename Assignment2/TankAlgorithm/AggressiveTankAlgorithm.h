#pragma once

#include "../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

class AggressiveTankAlgorithm : public AbstractTankAlgorithm
{
public:
    // === Constructor === //
    AggressiveTankAlgorithm(size_t player_index, size_t tank_index);

    // === Destructor === //
    ~AggressiveTankAlgorithm() override;

    // === Public Interface === //
    ActionRequest getActionLogic() override;

    void updateBattleInfo(BattleInfo &info) override;
};