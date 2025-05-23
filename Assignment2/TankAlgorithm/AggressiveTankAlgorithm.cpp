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

    // in first turn, get battle info
    if (this->getCurrentStep() == 0)
    {
        return ActionRequest::GetBattleInfo;
    }

    // search for enemy tanks

    // if enemy tank is in range and in line of sight, shoot

    // if enemy tank is not in range and in line of sight, move towards it

    // if enemy tank is not in range and not in line of sight, rotate towards it


    // escape from enemy shells

    // if enemy shell is in range and in line of sight, move away from it


    // shells run out
    if (this->getRemainingShells() == 0)
    {
        return ActionRequest::DoNothing;
    }

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
