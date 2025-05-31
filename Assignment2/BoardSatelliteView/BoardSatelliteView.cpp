#include "BoardSatelliteView.h"

// === Constructor === //
BoardSatelliteView::BoardSatelliteView(const GameBoard &board) : width(board.getWidth()), height(board.getHeight())
{
    for (BoardCell c : board.getOccupiedCells())
    {
        auto objects = board.getObjectsOnCell(c);
        if (objects.empty())
            continue; // should not get here since c is an occupied cell. for safety reasons only.

        if (objects.size() > 1)
        {
            this->sat_view[c] = GameObjectType::SHELL; // assuming  that 2 objects can only co-exist on same cell if one is mine, and other is shell
            continue;
        }

        // if here, objects.size() == 1
        auto obj = *(objects.begin());
        this->sat_view[c] = obj->getObjectType();
    }
}

// === Setter For Manager - Not From Interface === //
void BoardSatelliteView::setCallerTankLocation (const BoardCell& c){
    this->caller_tank_location = c;
}

// === Interface Implementation === //
char BoardSatelliteView::getObjectAt(size_t x, size_t y) const
{

    if (x > this->width || y > this->height) // not checking < 0 since size_t is unsigned
        return static_cast<char>(SpecialSatelliteViewChars::OutOfBounds);

    if (BoardCell(x,y) == caller_tank_location)
        return static_cast<char>(SpecialSatelliteViewChars::CallerTank);

    auto iter = this->sat_view.find(BoardCell(x, y));
    if (iter == this->sat_view.end())
        return static_cast<char>(SpecialSatelliteViewChars::EmptySpace);

    return static_cast<char>(iter->second);
}
