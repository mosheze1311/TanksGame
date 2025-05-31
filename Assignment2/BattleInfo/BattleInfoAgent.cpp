#include "BattleInfoAgent.h"

// === Constructor === //
BattleInfoAgent::BattleInfoAgent(SatelliteAnalyticsView &advanced_sat_view, const SatelliteView &sat_view, const PlayerToTankDetails& player_to_tank, const GameDetails& details)
    : game_details(details),

      advanced_sat_view(advanced_sat_view),
      new_satellite_image(sat_view),

      player_to_tank(player_to_tank){}

// === Getters (for Player) === //
size_t BattleInfoAgent::getTankIndex() const { return tank_to_player.tank_index; }

size_t BattleInfoAgent::getCurrentStep() const { return tank_to_player.current_step; }

size_t BattleInfoAgent::getRemainingShells() const { return tank_to_player.remaining_shells; }

Direction BattleInfoAgent::getTankDirection() const { return tank_to_player.dir; }

// === Getters (for TankAlgorithm) === //
BoardCell BattleInfoAgent::getTankLocation() const
{
    return player_to_tank.caller_tank_location;
}

size_t BattleInfoAgent::getMaxSteps() const { return game_details.max_steps; }

size_t BattleInfoAgent::getInitialNumShells() const { return game_details.initial_num_of_shells; }

size_t BattleInfoAgent::getStepToGetInfo(size_t current_step) const
{

    return current_step + player_to_tank.steps_gap_for_get_info;
}

// === Setters (for TankAlgorithm) === //
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

// === Two-Steps Update View API === //
//private
void BattleInfoAgent::updateViewForStep(size_t current_step)
{
    this->advanced_sat_view.updateAnalyticalView(this->new_satellite_image, current_step);
}

// public
SatelliteAnalyticsView BattleInfoAgent::updateAndGetAnalyticsView(size_t current_step)
{
    this->updateViewForStep(current_step);
    return this->advanced_sat_view;
}