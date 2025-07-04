#include "BoardSatelliteView.h"
namespace UserCommon_211388913_322330820
{

    // === Constructor === //
    BoardSatelliteView::BoardSatelliteView() {};
    BoardSatelliteView::BoardSatelliteView(const AbstractGameBoardView &board) : width(board.getWidth()), height(board.getHeight())
    {
        // if more than one object on cell, and one of the objects is a shell - saves the shell.
        // else - saves the first one in the unordered-set.
        for (BoardCell c : board.getOccupiedCells())
        {
            auto objects_types = board.getObjectsTypesOnCell(c);
            if (objects_types.empty())
                continue; // should not get here since c is an occupied cell. for safety reasons only.

            bool is_shell = objects_types.find(GameObjectType::SHELL) != objects_types.end();
            if (objects_types.size() > 1 && is_shell)
            {
                this->sat_view[c] = GameObjectType::SHELL;
                continue;
            }

            // if here, either there is only one object, or multiple without a shell (irrelevant to rules)
            auto obj_type = *(objects_types.begin());
            this->sat_view[c] = obj_type;
        }
    }

    // === Setter For Manager - Not From Interface === //
    void BoardSatelliteView::setCallerTankLocation(const BoardCell &c)
    {
        this->caller_tank_location = c;
    }
    void BoardSatelliteView::setWidth(size_t width) { this->width = width; };
    void BoardSatelliteView::setHeight(size_t height) { this->height = height; };
    void BoardSatelliteView::addObjectAt(const BoardCell &c, GameObjectType t)
    {
        this->sat_view[c] = t;
    };
    // === Interface Implementation === //
    char BoardSatelliteView::getObjectAt(size_t x, size_t y) const
    {
        if (x >= this->width || y >= this->height) // not checking < 0 since size_t is unsigned
            return static_cast<char>(SpecialSatelliteViewChars::OutOfBounds);

        if (BoardCell(x, y) == caller_tank_location)
            return static_cast<char>(SpecialSatelliteViewChars::CallerTank);

        auto iter = this->sat_view.find(BoardCell(x, y));
        if (iter == this->sat_view.end())
            return static_cast<char>(SpecialSatelliteViewChars::EmptySpace);

        return static_cast<char>(iter->second);
    }
}