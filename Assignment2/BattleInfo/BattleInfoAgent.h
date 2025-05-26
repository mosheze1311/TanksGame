#pragma once

#include "../common/ActionRequest.h"
#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"

#include "../GameBoard/GameBoard.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"

#include <optional>
#include <queue>
#include <vector>

// === Container Structs ===
struct GameDetails
{
    size_t max_steps;
    size_t board_rows;
    size_t board_cols;
    size_t initial_num_of_shells;

    GameDetails(size_t max_steps, size_t board_rows, size_t board_cols, size_t initial_num_of_shells)
        : max_steps(max_steps), board_rows(board_rows), board_cols(board_cols), initial_num_of_shells(initial_num_of_shells) {};
};

struct TankToPlayerDetails
{
    size_t tank_index;
    size_t current_step;
    size_t remaining_shells;
    Direction dir;

    // no need for constructor - updated by algorithm only after init
    };

struct PlayerToTankDetails
{
    BoardCell caller_tank_location;
    size_t steps_gap_for_get_info;

    PlayerToTankDetails(BoardCell &caller_tank_location, size_t steps_gap_for_get_info)
        : caller_tank_location(caller_tank_location), steps_gap_for_get_info(steps_gap_for_get_info) {}
};

// === Class Declaration === //
class BattleInfoAgent : public BattleInfo
{
private:
    // === Attributes === //
    GameDetails game_details;

    SatelliteAnalyticsView &advanced_sat_view;
    SatelliteView &new_satellite_image;

    PlayerToTankDetails player_to_tank;
    TankToPlayerDetails tank_to_player;

public:
    // === Constructor === //
    BattleInfoAgent(SatelliteAnalyticsView &advanced_sat_view, SatelliteView &sat_view, PlayerToTankDetails player_to_tank, GameDetails details);

    // === Destructor === //
    ~BattleInfoAgent() override;

    // === Getters (for Player) === //
    size_t getCurrentStep() const;

    size_t getTankIndex() const;
    Direction getTankDirection() const;
    size_t getRemainingShells() const;

    // === Getters (for Algorithm) === //
    size_t getMaxSteps() const;
    size_t getInitialNumShells() const;

    BoardCell getTankLocation() const;

    size_t getStepToGetInfo(size_t current_step) const;
    SatelliteAnalyticsView getAnalyticsView() const;

    // === Setters (for Player) === //
    // currently none: all setters are through the constructor

    // === Setters (for Algorithm) === //
    void setCurrentStep(size_t step);
    void setTankDirection(Direction dir);
    void setRemainingShells(size_t shells);

    // === Two-Steps Update View API === //
    // TODO: move this logic to battle info boject so that SatelliteAnalyitcsView wont save SatelliteView refernce as an attribute
    void updateViewForStep(size_t steps_gap);
};