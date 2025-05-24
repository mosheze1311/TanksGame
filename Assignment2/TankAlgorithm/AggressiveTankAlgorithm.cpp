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
    this->advance_step();
    if (this->getCurrentStep() == 1)
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

void AggressiveTankAlgorithm::updateBattleInfo(BattleInfo &info)
{
    auto *current_info = dynamic_cast<BattleInfoAgent *>(&info);
    if (!current_info)
        return; // Should never happen

    if (this->getCurrentStep() == 1)
    {
        this->setCurrentLocation(current_info->getCurrentCell());
        this->setRemainingShells(current_info->getMaxShells());
        
        current_info->updateViewForStep(this->getCurrentStep());
        this->sat_view = current_info->getAnalyticsView();
    }
    // else{

    // }
}