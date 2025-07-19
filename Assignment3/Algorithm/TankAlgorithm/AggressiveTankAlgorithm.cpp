#include "AggressiveTankAlgorithm.h"
namespace Algorithm_211388913_322330820
{
    // === Constructor === //
    AggressiveTankAlgorithm::AggressiveTankAlgorithm(int player_index, int tank_index)
        : AbstractTankAlgorithm(
              tank_index,  // tank_idx
              player_index // player_idx
          )
    {
    }

    // === clone === //
    std::unique_ptr<AbstractTankAlgorithm> AggressiveTankAlgorithm::clone() const
    {
        return std::make_unique<AggressiveTankAlgorithm>(*this);
    }

    // === Public Methods === //
    ActionRequest AggressiveTankAlgorithm::getActionLogic() const
    {
        // get battle info on required steps
        if (this->getCurrentStep() == this->step_to_get_info)
        {
            return ActionRequest::GetBattleInfo;
        }

        // Try to escape from incoming enemy shells
        if (auto escape_shell_action = escapeShells())
        {
            return escape_shell_action.value();
        }

        // Make aggresive action
        return this->getTankAggressiveAction();
    }

   
}