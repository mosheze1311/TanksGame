#include "GameBoardUtils.h"

// === Distance Functions === //
int GameBoardUtils::distance(const BoardCell &first, const BoardCell &second, size_t width, size_t height)
{
    return std::max(xDistance(first, second, width), yDistance(first, second, height));
}

int GameBoardUtils::xDistance(const BoardCell &first, const BoardCell &second, size_t width)
{
    size_t dx = abs(first.getX() - second.getX());
    dx = std::min(dx, width - dx);
    return dx;
}

int GameBoardUtils::yDistance(const BoardCell &first, const BoardCell &second, size_t height)
{
    size_t dy = abs(first.getY() - second.getY());
    dy = std::min(dy, height - dy);
    return dy;
}

// === Line, Direction and Neighbors Logic === //
bool GameBoardUtils::isStraightLine(const BoardCell &from, const BoardCell &to, size_t width, size_t height)
{
    int x_diff = xDistance(from, to, width);
    int y_diff = yDistance(from, to, height);

    if (x_diff == 0 || y_diff == 0)
    {
        return true;
    }
    if (abs(x_diff) == abs(y_diff))
    {
        return true;
    }

    return false;
}

bool GameBoardUtils::isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir, size_t width, size_t height)
{
    return getNextCellInDirection(from, dir, width, height) == getNextCellInStraightLine(from, to, width, height);
}

BoardCell GameBoardUtils::getNextCellInStraightLine(const BoardCell &from, const BoardCell &to, size_t width, size_t height)
{
    // assuming that from, to form a straight line in some direction.
    // TODO: try to write it in a algebraic form
    // int normal_dx = to.getX() - from.getX();
    // int normal_dy = to.getY() - from.getY();

    // int vertical_border_dx = this->getWidth() + min(from.getX(), to.getX()) - max(from.getX(), to.getX());
    // int horizontal_border_dy = this->getHeight() + min(from.getY(), to.getY()) - max(from.getY(), to.getY());

    std::vector<BoardCell> neighbors = getAdjacentCells(from, width, height);
    int min_dist = distance(from, to, width, height);
    BoardCell next = from;

    for (BoardCell neighbor : neighbors)
    {
        int neighbor_dist = distance(neighbor, to, width, height);
        if (neighbor_dist < min_dist && isStraightLine(neighbor, to, width, height))
        {
            min_dist = neighbor_dist;
            next = neighbor;
        }
    }
    return next;
}

BoardCell GameBoardUtils::getNextCellInDirection(const BoardCell &c, const Direction dir, size_t width, size_t height)
{
    return createAdjustedBoardCell(c + dir, width, height);
}

std::vector<BoardCell> GameBoardUtils::getAdjacentCells(const BoardCell &curr_cell, size_t width, size_t height)
{
    std::vector<BoardCell> res;

    for (int dir_number = 0; dir_number < 8; dir_number++)
    {
        Direction dir = static_cast<Direction>(dir_number);
        BoardCell neighbor = createAdjustedBoardCell(curr_cell + dir, width, height);
        res.push_back(neighbor);
    }

    return res;
}

// === BoardCell Normalization === //
BoardCell GameBoardUtils::createAdjustedBoardCell(const BoardCell &c, int width, int height)
{
    // The width, height parameters are signed int to prevent implicit conversion of the X,Y cell attributes to unsigned when performing % operation
    return BoardCell(((c.getX() % width) + width) % width, ((c.getY() % height) + height) % height);
}