#pragma once

#include "../common/SatelliteView.h"

#include "SpecialSatelliteViewChars.h"

#include "../GameBoard/GameBoard.h"

#include <vector>
#include <map>

class BoardSatelliteView : public SatelliteView
{
private:
    // === Attributes === //
    std::map<BoardCell, GameObjectType> sat_view;
    BoardCell caller_tank_location;
    const size_t width;
    const size_t height;

public:
    // === Constructor === //
    explicit BoardSatelliteView(const GameBoard &board);

    // === Setter For Manager - Not From Interface === //
    void setCallerTankLocation(const BoardCell& c);
 
    // === Interface Implementation === //
    char getObjectAt(size_t x, size_t y) const override;
};