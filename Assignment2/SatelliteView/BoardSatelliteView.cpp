#include "BoardSatelliteView.h"

//=== Constructor ===
BoardSatelliteView::BoardSatelliteView(GameBoard &board)
{
    for (BoardCell c : board.getOccupiedCells())
    {
        auto objects = board.getObjectsOnCell(c);
        if (objects.empty())
            continue; // should not get here since c is an occupied cell. for safety reasons only.

        auto obj = *(objects.begin());
        this->sat_view[c] = obj->getObjectType();
    }
}

//=== Interface Implementation ===
char BoardSatelliteView::getObjectAt(size_t x, size_t y) const
{
    // TODO: i don't like the use of a constant char ' ', we need to make it cleaner somehow
    auto iter = this->sat_view.find(BoardCell(x, y));
    if (iter == this->sat_view.end())
    {
        return ' ';
    }

    return static_cast<char>(iter->second);
}
