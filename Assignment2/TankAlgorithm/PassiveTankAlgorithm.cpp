#include "PassiveTankAlgorithm.h"

// === Constructor === //
PassiveTankAlgorithm::PassiveTankAlgorithm(size_t player_index, size_t tank_index)
    : AbstractTankAlgorithm(
          tank_index,       // tank_idx
          player_index    // player_idx
        )             
{}

// === Public Methods === //
ActionRequest PassiveTankAlgorithm::getActionLogic() const
{
    // TODO: Improve behavior

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
    if (auto shoot_action_opt = this->evaluateShootingOpportunity(approx_closet_enemy, sat_view.getWidth(), sat_view.getHeight()))
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
