#include "Tactic.h"
#include "../TankAlgorithm/CompetitionTankAlgorithm.h"

namespace Algorithm_211388913_322330820
{
    // === Bait Tactic === //
    Bait::Bait(const BoardCell &base_location, const BoardCell &enemy_location, bool base_is_ready)
        : base_location(base_location), enemy_location(enemy_location), base_is_ready(base_is_ready)
    {
    }

    ActionRequest Bait::getNextTacticAction(const CompetitionTankAlgorithm &algo)
    {
        int safety_gap = 3;
        int dist_from_base = algo.sat_view.distance(algo.getCurrentLocation(), base_location);
        if (!base_is_ready && dist_from_base > safety_gap)
        {
            return algo.advanceTankToTarget(base_location);
        }

        int dist_from_enemy = algo.sat_view.distance(algo.getCurrentLocation(), enemy_location);
        if (dist_from_enemy > safety_gap &&
            dist_from_base + safety_gap <= dist_from_enemy)
        {
            return algo.advanceTankToTarget(enemy_location);
        }

        if (dist_from_enemy < safety_gap)
        {
            return algo.advanceTankToTarget(base_location);
        }

        return ActionRequest::GetBattleInfo;
    }

    // === Camper Tactic === //
    Camper::Camper(const BoardCell &base_location, const BoardCell &enemy_location) : base_location(base_location), enemy_location(enemy_location) {}

    ActionRequest Camper::getNextTacticAction(const CompetitionTankAlgorithm &algo)
    {
        int safety_gap = 3;

        int dist_from_base = algo.sat_view.distance(algo.getCurrentLocation(), this->base_location);
        int dist_from_enemy = algo.sat_view.distance(algo.getCurrentLocation(), this->enemy_location);

        if (dist_from_base > safety_gap)
        {
            return algo.advanceTankToTarget(base_location);
        }

        std::optional<BoardCell> opt_next_cell = algo.findNextCellInPath(algo.getCurrentLocation(), enemy_location);
        if (opt_next_cell == std::nullopt || dist_from_enemy > safety_gap * safety_gap)
        {
            return ActionRequest::GetBattleInfo; // if cant reach enemy target, getBattleInfo
        }

        BoardCell next_cell_towards_enemy = *opt_next_cell;
        ActionRequest direction_fix = algo.adjustDirection(next_cell_towards_enemy);
        if (direction_fix == ActionRequest::DoNothing)
        {
            return ActionRequest::Shoot;
        }

        return direction_fix;
    }

    // === ScaredWolf Tactic === //
    ActionRequest ScaredWolf::getNextTacticAction(const CompetitionTankAlgorithm &algo)
    {
        return algo.AbstractTankAlgorithm::getBestProbSurvivalAction(algo, 3, ActionRequest::GetBattleInfo).first;
    };

    // === Attaker Tactic === //
    ActionRequest Attacker::getNextTacticAction(const CompetitionTankAlgorithm &algo)
    {
        return algo.getTankAggressiveAction();
    }
}