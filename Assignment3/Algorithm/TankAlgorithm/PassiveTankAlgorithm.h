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
    PassiveTankAlgorithm(int player_index, int tank_index);

    // === Destructor === //
    ~PassiveTankAlgorithm() override = default;
};