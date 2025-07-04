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

    // === Aggresive Algorithm === //
    ActionRequest AggressiveTankAlgorithm::getTankAggressiveAction() const
    {
        // try to chase the enemy tank or shoot at it.

        // dijkstra to closest tank
        // if can shoot it - shoot
        // if can't shoot it - chase

        BoardCell start = this->assumed_location;
        BoardCell target = this->approxClosestEnemyTankLocation();

        // try to shoot enemy
        if (auto shoot_action_opt = this->evaluateShootingOpportunity(target))
        {
            return shoot_action_opt.value();
        }

        // if will be able to shoot in the future, wait
        if (this->inShootRange(target) &&
            sat_view.isStraightLine(start, target) && this->getRemainingShells() > 0)
        {
            return ActionRequest::DoNothing;
        }

        return advanceTankToTarget(target);
    }
}