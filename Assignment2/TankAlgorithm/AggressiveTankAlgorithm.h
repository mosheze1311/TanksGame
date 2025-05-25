#pragma once

#include "../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

class AggressiveTankAlgorithm : public AbstractTankAlgorithm
{
private:
    // inner logic function for the getAction interface
    ActionRequest getActionLogic() override;

public:
    // === Constructor === //
    AggressiveTankAlgorithm(size_t player_index, size_t tank_index);

    // === Destructor === //
    ~AggressiveTankAlgorithm() override;

};