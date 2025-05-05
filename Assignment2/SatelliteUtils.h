#pragma once
#include "SatelliteView.h"
#include "GameBoard.h"
#include <vector>
#include <map>

using std::map;

class BoardSatelliteView : public SatelliteView
{
private:
    //=== Attributes ===
    map<BoardCell, GameObjectType> sat_view;

public:
    explicit BoardSatelliteView(GameBoard &board)
    {
        for (BoardCell c : board.getOccupiedCells()){
            auto objects = board.getObjectsOnCell(c);
            
            if(objects.empty())
                continue; // should never reach here because BoardCell is occupied. here for safety reasons only.

            this->sat_view[c] = objects[0]->getObjectType();
        }
    }

    char getObjectAt(size_t x, size_t y) const override
    {
        // TODO: i don't like the use of a constant char ' ', we need to make it cleaner somehow
        auto iter = this->sat_view.find(BoardCell(x, y));
        if (iter == this->sat_view.end())
        {
            return ' ';
        }

        return static_cast<char>(iter->second);
    }
};