#include "AggressiveTankAlgorithm.h"

// === Constructor === //
AggressiveTankAlgorithm::AggressiveTankAlgorithm(size_t player_index, size_t tank_index)
    : AbstractTankAlgorithm(
          tank_index,              // tank_idx
          player_index            // player_idx
    )
{}

// === Destructor === //
AggressiveTankAlgorithm::~AggressiveTankAlgorithm() = default;

// === Public Methods === //
ActionRequest AggressiveTankAlgorithm::getActionLogic()
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

    // Make aggresive action
    return this->getTankAggressiveAction(this->sat_view);
}
