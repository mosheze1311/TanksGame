#include "../common/Player.h"

#include <vector>

#include "../GameBoard/GameBoard.h"

#include "../BattleInfo/BattleInfoAgent.h"

#include <map>

class PlayerOne : public Player
{

private:
    int player_index;
    size_t board_width;
    size_t board_height;
    size_t max_steps;
    size_t initial_num_of_shells;

    size_t live_tanks_count;

    map<size_t, BoardCell> tanks_locations; // maps tank id to its location
    size_t current_step;
    SatelliteAnalyitcsView analytics_view;

    //=== Helper Functions ===
    BoardCell parseTankLocation(SatelliteView &satellite_view)
    {
        BoardCell caller_tank_location;
        for (size_t x = 0; x < this->board_width; ++x)
        {
            for (size_t y = 0; y < this->board_height; ++y)
            {
                if (satellite_view.getObjectAt(x, y) == '%')
                    return BoardCell(x, y);
            }
        }
        return caller_tank_location; // should never get here if SatelliteView is valid
    }
    PlayerToTankDetails createPlayerToTankDetails(SatelliteView &satellite_view)
    {
        size_t steps_gap_for_get_info = 100; // TODO: this is the new version - it is not exactly what we need...
        // size_t steps_gap_for_get_info = analytics_view.getMaxStepGap(); // TODO: this is the old version - it is not exactly what we need
        BoardCell caller_tank_location = parseTankLocation(satellite_view);
        PlayerToTankDetails pttd(caller_tank_location, steps_gap_for_get_info);
        return pttd;
    }

public:
    //=== Consturctor ===//
    PlayerOne(int player_index,
              size_t x, size_t y,
              size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells),
          board_width(x), board_height(y), max_steps(max_steps), initial_num_of_shells(num_shells), analytics_view(y, x, 5, player_index) {}

    //=== Destructor ===//
    ~PlayerOne() override {}

    //=== Player Interface ===//
    void updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view) override
    {
        // create BattleInfoAgent
        PlayerToTankDetails pttd = createPlayerToTankDetails(satellite_view);
        GameDetails game_details(this->max_steps, this->board_width, this->board_height, this->initial_num_of_shells);
        BattleInfoAgent battle_info(this->analytics_view, satellite_view, pttd, game_details);

        // send BattleInfoAgent to TankAlgorithm
        tank_algo.updateBattleInfo(battle_info);

        // save BattleInfoAgent recieved data (SatelliteAnalyitcsView is updated automatically)
        size_t tank_idx = battle_info.getTankIndex();
        this->tanks_locations[tank_idx] = pttd.caller_tank_location;
        this->current_step = battle_info.getCurrentStep();
        // TODO: decide on what to do with following values
        battle_info.getRemainingShells(); 
        battle_info.getTankDirection();
    }
};
