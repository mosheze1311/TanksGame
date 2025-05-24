#pragma once

#include "../common/SatelliteView.h"

#include "../GameBoard/BoardCell.h"
#include "../GameObjects/Direction.h"
#include "../GameObjects/GameObjectType.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"

#include <vector>
#include <map>

#define AnalyticalViewMap std::map<BoardCell, std::pair<GameObjectType, AssumedDirection>>

using namespace DirectionUtils; // for multiplying direction by a const

enum class AssumedDirection
{
    UP = 0,
    UPR = 1,
    RIGHT = 2,
    DOWNR = 3,
    DOWN = 4,
    DOWNL = 5,
    LEFT = 6,
    UPL = 7,
    UNKNOWN = 8
};

// TODO: the satellite images are taken before the beginning of the step. therefore, to accuratly represent the board, need to move shells another step?
// TODO: if above todo is made, how to handle bullets with unkown location?
// TODO: allow more than 1 object at cell so that object movements are more accurate
class SatelliteAnalyitcsView
{
private:
    // === Attributes === //
    size_t player_idx;
    size_t max_steps_gap;

    size_t width, height;
    AnalyticalViewMap analytical_view;
    size_t last_updated_step = 0;
    size_t enemy_estimate_tanks_num;

    std::vector<BoardCell> shells_locations;
    std::vector<BoardCell> enemies_locations;
    std::vector<BoardCell> mines_locations;

    // === INIT Helpers === //
    BoardCell shellExpectedLocation(BoardCell &old_location, AssumedDirection assumed_dir, size_t steps_gap);
    bool isDirectionMatchShell(BoardCell &old_location, BoardCell &new_location, AssumedDirection assumed_dir, size_t steps_gap);
    AssumedDirection findMatchingShellDirection(BoardCell &old_location, size_t steps_gap);

    // === INIT === //
    void initAnalyticalView(SatelliteView &sat_view);

    // === Move Objects on View === //
    void advanceShells();
    void advanceShellsOnce();

public:
    // === Constructor === //
    SatelliteAnalyitcsView(size_t height, size_t width, size_t max_steps_gap, size_t player_idx);

    //=== Destructor ===//
    ~SatelliteAnalyitcsView();

    //=== Getters ===//
    size_t getWidth() const;
    size_t getHeight() const;
    size_t getMaxStepGap() const;
    size_t getEnemyTanksNum() const;
    std::vector<BoardCell> getEnemyTanksLocations() const;
    std::vector<BoardCell> getShellsLocations() const;
    std::vector<BoardCell> getMinesLocations() const;
    std::pair<char, AssumedDirection> getObjectAt(size_t x, size_t y) const;


    // === UPDATE === //
    void updateAnalyticalView(SatelliteView &sat_view, size_t current_step);

    // this function moves objects on board to allow algorithm to use the view in current step.
    void approxBoardChanges();
};
