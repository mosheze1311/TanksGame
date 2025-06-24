#pragma once

#include "../../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    class AggressiveTankAlgorithm : public AbstractTankAlgorithm
    {
    private:
        // inner logic function for the getAction interface
        ActionRequest getActionLogic() const override;

        // === Aggresive Algorithm === //
        ActionRequest getTankAggressiveAction() const;

    public:
        // === Constructor === //
        AggressiveTankAlgorithm(int player_index, int tank_index);

        // === Destructor === //
        ~AggressiveTankAlgorithm() override = default;
    };
}