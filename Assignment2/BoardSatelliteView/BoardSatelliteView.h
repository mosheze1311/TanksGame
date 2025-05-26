#pragma once

#include "../common/SatelliteView.h"

#include "../GameBoard/GameBoard.h"

#include <vector>
#include <map>

using std::map;

class BoardSatelliteView : public SatelliteView
{
private:
    //=== Attributes ===
    map<BoardCell, GameObjectType> sat_view;
    BoardCell caller_tank_location;
    size_t width;
    size_t height;

    static const char empty_space = ' ';
    static const char caller_tank = '%';
    static const char out_of_bounds = '&';

public:
    //=== Constructor ===
    explicit BoardSatelliteView(GameBoard &board);

    //=== Public Functions For Manager - Not From Interface ===
    void setCallerTankLocation(const BoardCell& c);

    //=== Interface Implementation ===
    char getObjectAt(size_t x, size_t y) const override;
};