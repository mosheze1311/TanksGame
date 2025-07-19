#pragma once

#include "../../common/TankAlgorithm.h"

#include "AbstractTankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"

namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    class PassiveTankAlgorithm : public AbstractTankAlgorithm
    {
    private:
        ActionRequest getActionLogic() const override;

    public:
        // === Constructor === //
        PassiveTankAlgorithm(int player_index, int tank_index);

        // === Destructor === //
        ~PassiveTankAlgorithm() override = default;

        // === Clone === //
        std::unique_ptr<AbstractTankAlgorithm> clone() const override;
        
    };
}