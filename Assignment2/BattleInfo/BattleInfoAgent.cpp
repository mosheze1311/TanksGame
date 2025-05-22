#include "BattleInfoAgent.h"

// === Constructor ===
BattleInfoAgent::BattleInfoAgent(SatelliteAnalyitcsView &advanced_sat_view, PlayerToTankDetails player_to_tank, GameDetails details)
    : advanced_sat_view(advanced_sat_view),
      player_to_tank(player_to_tank),
      game_details(details) {}

// === Destructor ===
BattleInfoAgent::~BattleInfoAgent() {} // TODO: write implementation if needed

// === Getters (for TankAlgorithm) ===
BoardCell BattleInfoAgent::getCurrentCell() const
{
    return player_to_tank.current_cell;
}

size_t BattleInfoAgent::getEstimatedEnemyRemainingShells() const { return player_to_tank.estimated_enemy_remaining_shells; }

size_t BattleInfoAgent::getMaxSteps() const { return game_details.max_steps; }

// === Getters (for Player) ===
BoardCell BattleInfoAgent::getEnemyTargetLocation() const { return tank_to_player.enemy_target_location; }

size_t BattleInfoAgent::getTankIndex() const { return tank_to_player.tank_index; }

size_t BattleInfoAgent::getCurrentStep() const { return tank_to_player.current_step; }

size_t BattleInfoAgent::getRemainingShells() const { return tank_to_player.remaining_shells; }

Direction BattleInfoAgent::getTankDirection() const { return tank_to_player.dir; }


// === Setters (for TankAlgorithm) ===
void BattleInfoAgent::setEnemyTargetLocation(const BoardCell& target)
{
    tank_to_player.enemy_target_location = target;
}

void BattleInfoAgent::setCurrentStep(size_t step)
{
    tank_to_player.current_step = step;
}

void BattleInfoAgent::setRemainingShells(size_t shells)
{
    tank_to_player.remaining_shells = shells;
}

void BattleInfoAgent::setTankDirection(Direction dir)
{
    tank_to_player.dir = dir;
}

// === Setters (for Player) ===
void BattleInfoAgent::setCurrentCell(BoardCell const& cell)
{
    player_to_tank.current_cell = cell;
}

void BattleInfoAgent::setEstimatedEnemyRemainingShells(size_t shells)
{
    player_to_tank.estimated_enemy_remaining_shells = shells;
}
