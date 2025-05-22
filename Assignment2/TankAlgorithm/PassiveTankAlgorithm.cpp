#include "PassiveTankAlgorithm.h"

// === Constructor === //
PassiveTankAlgorithm::PassiveTankAlgorithm(size_t player_index, size_t tank_index)
    : AbstractTankAlgorithm(
          tank_index,       // tank_idx
          player_index,     // player_idx
          0,                // num_of_shells
          BoardCell(0, 0),  // assumed_location
          Direction::RIGHT, // direction
          0)                // current_step
{
}

// === Destructor === //
PassiveTankAlgorithm::~PassiveTankAlgorithm() = default;

// === Public Methods === //
ActionRequest PassiveTankAlgorithm::getAction()
{
    return ActionRequest::MoveForward;
}

void PassiveTankAlgorithm::updateBattleInfo(BattleInfo &info)
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
