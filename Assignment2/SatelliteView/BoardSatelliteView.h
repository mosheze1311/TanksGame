#pragma once
#include "../common/SatelliteView.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include <vector>
#include <map>

using std::map;

class BoardSatelliteView : public SatelliteView
{
private:
    //=== Attributes ===
    map<BoardCell, GameObjectType> sat_view;
    size_t height;
    size_t width;

    static const char empty_space = ' ';
    static const char out_of_bounds = '&';
    
public:
    //=== Constructor ===
    explicit BoardSatelliteView(GameBoard &board);
    
    //=== Interface Implementation ===
    char getObjectAt(size_t x, size_t y) const override;
};