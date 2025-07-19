#include "PassiveTankAlgorithm.h"
namespace Algorithm_211388913_322330820
{
    // === Constructor === //
    PassiveTankAlgorithm::PassiveTankAlgorithm(int player_index, int tank_index)
        : AbstractTankAlgorithm(
              tank_index,  // tank_idx
              player_index // player_idx
          )
    {
    }

    // === Public Methods === //
    ActionRequest PassiveTankAlgorithm::getActionLogic() const
    {
        // in first turn, get battle info
        if (this->getCurrentStep() == this->step_to_get_info)
        {
            return ActionRequest::GetBattleInfo;
        }

        // Try to escape from incoming enemy shells
        if (auto escape_shell_action = escapeShells())
        {
            return escape_shell_action.value();
        }

        // If enemy in range, try to escape
        BoardCell approx_closet_enemy = this->approxClosestEnemyTankLocation();
        if (auto shoot_action_opt = this->evaluateShootingOpportunity(approx_closet_enemy))
        {
            return shoot_action_opt.value();
        }

        // shells run out
        if (this->getRemainingShells() == 0)
        {
            return ActionRequest::DoNothing;
        }

        return ActionRequest::Shoot;
    }

    std::unique_ptr<AbstractTankAlgorithm> PassiveTankAlgorithm::clone() const
    {
        return std::make_unique<PassiveTankAlgorithm>(*this);
    }
}