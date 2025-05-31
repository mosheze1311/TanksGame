#pragma once

#include "../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

class PassiveTankAlgorithm : public AbstractTankAlgorithm
{
private:
    ActionRequest getActionLogic() const override;

public:
    // === Constructor === //
    PassiveTankAlgorithm(size_t player_index, size_t tank_index);

    // === Destructor === //
    ~PassiveTankAlgorithm() override = default;
};