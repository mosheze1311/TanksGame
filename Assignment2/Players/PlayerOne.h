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

public:
    //=== Consturctor ===//
    PlayerOne(int player_index,
              size_t x, size_t y,
              size_t max_steps, size_t num_shells)
        : Player(player_index, x, y, max_steps, num_shells), analytics_view(y,x,0,player_index){}

    //=== Destructor ===//
    ~PlayerOne() override {}

    //=== Player Logic ===//
    void updateTankWithBattleInfo(TankAlgorithm &tank_algo, SatelliteView &satellite_view) override
    {
        // references for tank to update
        size_t tank_idx;
        size_t current_step;
        size_t remaining_shells;
        size_t enenmy_estimated_shells = analytics_view.getEnemyTanksNum();
        size_t step_to_get_info = analytics_view.getMaxStepGap();

        BoardCell tank_location = parseSatViewLocation(satellite_view);
        PlayerToTankDetails pttd(tank_location, enenmy_estimated_shells, step_to_get_info);
        GameDetails game_details(this->max_steps,this->board_width, this->board_height, this->initial_num_of_shells);
        BattleInfoAgent battle_info(analytics_view, satellite_view, pttd, game_details);

        tank_algo.updateBattleInfo(battle_info);

        this->tanks_locations[tank_idx] = tank_location;
        this->current_step = current_step;
        
    }

    BoardCell parseSatViewLocation(SatelliteView &satellite_view)
    {
        // TODO: extract more data / return inside loop
        BoardCell caller_tank_location;
        for (size_t x = 0; x < this->board_width; ++x)
        {
            for (size_t y = 0; y < this->board_height; ++y)
            {
                if(satellite_view.getObjectAt(x,y) == '%')
                    caller_tank_location = BoardCell(x,y);                
            }
        }

        return caller_tank_location;
    }
};
