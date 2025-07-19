#include "AbstractPlayer.h"
namespace Algorithm_211388913_322330820
{
    // === Consturctor === //
    AbstractPlayer::AbstractPlayer(int player_index,
                                   size_t x, size_t y,
                                   size_t max_steps, size_t num_shells)
        : player_index(player_index), board_width(x), board_height(y), max_steps(max_steps), initial_num_of_shells(num_shells),
          analytics_view(y, x, analysis_max_steps_gap, player_index)
    {
    }

    // === Helper Functions === //
    void AbstractPlayer::parseSatelliteView(const SatelliteView &satellite_view)
    {
        my_tanks_locations.clear();
        for (size_t x = 0; x < this->board_width; ++x)
        {
            for (size_t y = 0; y < this->board_height; ++y)
            {
                if (satellite_view.getObjectAt(x, y) == static_cast<char>(SpecialSatelliteViewChars::CallerTank))
                {
                    this->caller_tank_location = BoardCell(x, y);
                    my_tanks_locations.insert(caller_tank_location);
                }

                if (satellite_view.getObjectAt(x, y) == static_cast<char>(GameObjectTypeUtils::playerIndexToTankType(this->player_index)))
                {
                    my_tanks_locations.insert(BoardCell(x,y));
                }
            }
        }
    }

    PlayerToTankDetails AbstractPlayer::createPlayerToTankDetails()
    {
        size_t steps_gap_for_get_info = calculateStepsGapForGetInfo();
        PlayerToTankDetails pttd(caller_tank_location, steps_gap_for_get_info, this->formations);
        return pttd;
    }

    // === Player Interface === //
    void AbstractPlayer::updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view)
    {
        parseSatelliteView(satellite_view);
        this->formations = Formation::createFormationGroups(my_tanks_locations,
                                                            [this](const BoardCell &first, const BoardCell &second)
                                                            {
                                                                return this->analytics_view.distance(first, second);
                                                            });
        PlayerToTankDetails pttd = createPlayerToTankDetails();
        GameDetails game_details(this->max_steps, this->board_height, this->board_width, this->initial_num_of_shells);
        BattleInfoAgent battle_info(this->analytics_view, satellite_view, pttd, game_details);

        tank_algo.updateBattleInfo(battle_info);

        this->current_step = battle_info.getCurrentStep();
        this->tanks_details.emplace(battle_info.getTankIndex(), TankDetails(
                                                                    battle_info.getRemainingShells(),
                                                                    caller_tank_location,
                                                                    battle_info.getTankDirection(),
                                                                    battle_info.getTactic()));

    }
}