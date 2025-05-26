#include "PassiveTankAlgorithm.h"

// === Constructor === //
PassiveTankAlgorithm::PassiveTankAlgorithm(size_t player_index, size_t tank_index)
    : AbstractTankAlgorithm(
          tank_index,       // tank_idx
          player_index    // player_idx
        )             
{}

// === Destructor === //
PassiveTankAlgorithm::~PassiveTankAlgorithm() = default;

// === Public Methods === //
ActionRequest PassiveTankAlgorithm::getActionLogic()
{
    // TODO: Implement actual behavior

    // in first turn, get battle info
    if (this->getCurrentStep() == this->step_to_get_info)
    {
        return ActionRequest::GetBattleInfo;
    }

    // Try to escape from incoming enemy shells
    if (auto escape_shell_action = escapeShells(sat_view))
    {
        return escape_shell_action.value();
    }

    // If enemy in range, try to escape
    BoardCell approx_closet_enemy = this->approxClosestEnemyTankLocation(sat_view);
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
