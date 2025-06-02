#pragma once

#include "../GameBoard/BoardCell.h"

#include <vector>
class GameBoardUtils
{
private:
    GameBoardUtils() = delete;
    ~GameBoardUtils() = delete;

public:
    // === Distance Functions === //
    // get distance of 2 cells
    static int distance(const BoardCell &first, const BoardCell &second, size_t width, size_t height);

    // finds x distance
    static int xDistance(const BoardCell &first, const BoardCell &second, size_t width);

    // finds y distance
    static int yDistance(const BoardCell &first, const BoardCell &second, size_t height);

    // === Line, Direction and Neighbors Logic === //
    static bool isStraightLine(const BoardCell &from, const BoardCell &to, size_t width, size_t height);

    static bool isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir, size_t width, size_t height);

    static BoardCell getNextCellInStraightLine(const BoardCell& from, const BoardCell& to, size_t width, size_t height);

    static BoardCell getNextCellInDirection(const BoardCell &c, Direction dir, size_t width, size_t height);

    // get neigbhor cells
    static std::vector<BoardCell> getAdjacentCells(const BoardCell &curr_cell, size_t width, size_t height);

    // === BoardCell Normalization === //
    static BoardCell createAdjustedBoardCell(const BoardCell &c, int width, int height);
};
