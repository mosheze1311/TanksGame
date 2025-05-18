#pragma once

#include "../GameBoard/GameBoard.h"

class GameBoardUtils
{
private:
    GameBoardUtils() = delete;
    ~GameBoardUtils() = delete;

public:
    // get distance of 2 cells
    static int distance(const BoardCell &first, const BoardCell &second, size_t width, size_t height);

    // finds x distance
    static int xDistance(const BoardCell &first, const BoardCell &second, size_t width);

    // finds y distance
    static int yDistance(const BoardCell &first, const BoardCell &second, size_t height);

    static bool isStraightLine(BoardCell from, BoardCell to, size_t width, size_t height); // maybe add modulo

    static bool isDirectionMatch(BoardCell from, BoardCell to, Direction dir, size_t width, size_t height);

    static BoardCell getNextCellInStraightLine(BoardCell from, BoardCell to, size_t width, size_t height);

    // get neigbhor cells
    static std::vector<BoardCell> getAdjacentCells(const BoardCell &curr_cell, size_t width, size_t height);

    static BoardCell getNextCellInDirection(const BoardCell &c, const Direction dir, size_t width, size_t height);

    static BoardCell createAdjustedBoardCell(const BoardCell &c, size_t width, size_t height);
};
