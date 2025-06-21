#include "AbstractPlayer.h"



// === Consturctor === //
AbstractPlayer::AbstractPlayer(int player_index,
                               size_t x, size_t y,
                               size_t max_steps, size_t num_shells)
    : Player(player_index, x, y, max_steps, num_shells),
      board_width(x), board_height(y), max_steps(max_steps), initial_num_of_shells(num_shells),
      analytics_view(y, x, analysis_max_steps_gap, player_index) {}

// === Helper Functions === //
BoardCell AbstractPlayer::parseSatelliteView(const SatelliteView &satellite_view)
{
    tanks_alive = 1;
    BoardCell caller_tank_location;
    for (size_t x = 0; x < this->board_width; ++x)
    {
        for (size_t y = 0; y < this->board_height; ++y)
        {
            if (satellite_view.getObjectAt(x, y) == '%')
            {
                this->caller_tank_location = BoardCell(x, y);
            }

            if (satellite_view.getObjectAt(x, y) == static_cast<char>(GameObjectTypeUtils::playerIndexToTankType(this->player_index)))
            {
                tanks_alive++;
            }
        }
    }
    return caller_tank_location;
}

PlayerToTankDetails AbstractPlayer::createPlayerToTankDetails(const SatelliteView &satellite_view)
{
    parseSatelliteView(satellite_view);
    size_t steps_gap_for_get_info = calculateStepsGapForGetInfo();
    PlayerToTankDetails pttd(caller_tank_location, steps_gap_for_get_info);
    return pttd;
}

// === Player Interface === //
void AbstractPlayer::updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view)
{
    PlayerToTankDetails pttd = createPlayerToTankDetails(satellite_view);
    GameDetails game_details(this->max_steps, this->board_height, this->board_width, this->initial_num_of_shells);
    BattleInfoAgent battle_info(this->analytics_view, satellite_view, pttd, game_details);

    tank_algo.updateBattleInfo(battle_info);

    size_t tank_idx = battle_info.getTankIndex();
    this->tanks_locations[tank_idx] = pttd.caller_tank_location;
    this->current_step = battle_info.getCurrentStep();
    battle_info.getRemainingShells();
    battle_info.getTankDirection();
}
