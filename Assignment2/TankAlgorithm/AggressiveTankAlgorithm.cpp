#include "AggressiveTankAlgorithm.h"

// === Constructor === //
AggressiveTankAlgorithm::AggressiveTankAlgorithm(size_t player_index, size_t tank_index)
    : AbstractTankAlgorithm(
          tank_index,              // tank_idx
          player_index,            // player_idx
          0,                       // num_of_shells
          BoardCell(0, 0),         // assumed_location
          Direction::RIGHT,        // direction
          0)                       // current_step
{}

// === Destructor === //
AggressiveTankAlgorithm::~AggressiveTankAlgorithm() = default;

// === Public Methods === //
ActionRequest AggressiveTankAlgorithm::getAction()
{
    // TODO: Implement actual behavior
    return ActionRequest::Shoot;
}

void AggressiveTankAlgorithm::updateBattleInfo(BattleInfo &info)
{
    auto *current_info = dynamic_cast<BattleInfoAgent *>(&info);
    if (!current_info)
    {
        // Should never happen
        return;
    }

    this->setCurrentLocation(current_info->getCurrentCell());
    this->setRemainingShells(current_info->getRemainingShells());
    this->setCurrentStep(current_info->getCurrentStep());
    this->setTankDirection(current_info->getTankDirection());
    this->setTankIndex(current_info->getTankIndex());
}
