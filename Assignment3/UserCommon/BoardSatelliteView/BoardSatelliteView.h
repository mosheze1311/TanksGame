#pragma once

#include "../../common/SatelliteView.h"

#include "SpecialSatelliteViewChars.h"

#include "../GameBoard/GameBoard.h"

#include <vector>
#include <map>
namespace UserCommon_211388913_322330820
{

    class BoardSatelliteView : public SatelliteView
    {
    private:
        // === Attributes === //
        std::map<BoardCell, GameObjectType> sat_view;
        BoardCell caller_tank_location;
        size_t width;
        size_t height;

    public:
        // === Constructor === //
        BoardSatelliteView();
        explicit BoardSatelliteView(const GameBoard &board); // for GameManager

        // === Setters For Map Parser - Not From Interface === //
        void setCallerTankLocation(const BoardCell &c);
        void setWidth(size_t width);
        void setHeight(size_t height);
        void addObjectAt(const BoardCell &c, GameObjectType t);

        // === Interface Implementation === //
        char getObjectAt(size_t x, size_t y) const override;
    };
}