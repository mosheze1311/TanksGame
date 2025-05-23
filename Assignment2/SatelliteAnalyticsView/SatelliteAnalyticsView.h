#pragma once

#include "../common/SatelliteView.h"

#include "../GameBoard/BoardCell.h"
#include "../GameObjects/Direction.h"
#include "../GameObjects/GameObjectType.h"
#include "../SatelliteView/BoardSatelliteView.h"

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
class SatelliteAnalyitcsView
{
private:
    // === Attributes === //
    size_t player_idx;
    
    size_t width, height;
    AnalyticalViewMap analytical_view;
    SatelliteView &new_satellite_image;

    std::vector<BoardCell> shells_locations;
    std::vector<BoardCell> enemies_location;
    const size_t max_steps_gap;

    // === INIT Hellpers === //
    void initAnalyticalView(SatelliteView &sat_view);
    BoardCell shellExpectedLocation(BoardCell &old_location, AssumedDirection assumed_dir, size_t steps_gap);
    bool isDirectionMatchShell(BoardCell &old_location, BoardCell &new_location, AssumedDirection assumed_dir, size_t steps_gap);
    AssumedDirection findMatchingShellDirection(BoardCell &old_location, size_t steps_gap);
   
public:
    // === Constructor === //
    SatelliteAnalyitcsView(SatelliteView &sat_view, size_t height, size_t width, size_t max_steps_gap);

    //=== Destructor ===//
    ~SatelliteAnalyitcsView();

    // === Two-Steps Update View API ===
    // TODO: move this logic to battle info boject so that SatelliteAnalyitcsView wont save SatelliteView refernce as an attribute
    void uploadSatelliteImage(SatelliteView &sat_view);
    void updateViewForStep(size_t steps_gap);
    
    //=== Getters ===//
    size_t getWidth() const;
    size_t getHeight() const;
    std::vector<BoardCell> getEnemyTanksLocations() const;
    std::pair<char, AssumedDirection> getObjectAt(size_t x, size_t y) const;
};
