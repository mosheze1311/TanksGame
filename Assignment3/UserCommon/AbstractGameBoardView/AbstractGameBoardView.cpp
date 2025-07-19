#include "AbstractGameBoardView.h"

#include <utility>
namespace UserCommon_211388913_322330820
{
    // === BoardDetails Constructor === //
    AbstractGameBoardView::BoardDetails::BoardDetails(size_t width, size_t height, size_t max_steps, size_t num_shells) : width(width), height(height), max_steps(max_steps), tanks_num_shells(num_shells),
                                                                                                                          walls(0), mines(0), shells(0), remaining_shells(0) {};

    AbstractGameBoardView::AbstractGameBoardView(size_t width, size_t height, size_t max_steps, size_t num_shells) : board_details(width, height, max_steps, num_shells) {};

    // === Getters === //
    size_t AbstractGameBoardView::getWidth() const
    {
        return this->board_details.width;
    }

    size_t AbstractGameBoardView::getHeight() const
    {
        return this->board_details.height;
    }

    size_t AbstractGameBoardView::getMaxSteps() const
    {
        return this->board_details.max_steps;
    }

    size_t AbstractGameBoardView::getTanksNumShells() const
    {
        return this->board_details.tanks_num_shells;
    }

    std::map<GameObjectType, size_t> AbstractGameBoardView::getTanksCountPerType() const
    {
        return this->board_details.tanks_count;
    }

    int AbstractGameBoardView::getGameObjectCount(const GameObjectType type) const
    {
        switch (type)
        {
        case GameObjectType::TANK1:
        case GameObjectType::TANK2:
        case GameObjectType::TANK3:
        case GameObjectType::TANK4:
        case GameObjectType::TANK5:
        case GameObjectType::TANK6:
        case GameObjectType::TANK7:
        case GameObjectType::TANK8:
        case GameObjectType::TANK9:
            try
            {
                return this->board_details.tanks_count.at(type);
            }
            catch (const std::out_of_range &e)
            {
                return 0;
            }
        case GameObjectType::WALL:
            return this->board_details.walls;
        case GameObjectType::SHELL:
            return this->board_details.shells;
        case GameObjectType::MINE:
            return this->board_details.mines;

        default: // should not get here
            return 0;
        }
    }

    int AbstractGameBoardView::getTotalRemainingShells() const
    {
        return this->board_details.remaining_shells;
    }

    // === Setters === //
    void AbstractGameBoardView::setWidth(size_t width)
    {
        this->board_details.width = width;
    }

    void AbstractGameBoardView::setHeight(size_t height)
    {
        this->board_details.height = height;
    }

    void AbstractGameBoardView::setMaxSteps(size_t max_steps)
    {
        this->board_details.max_steps = max_steps;
    }

    void AbstractGameBoardView::setTanksNumShells(size_t tanks_num_shells)
    {
        this->board_details.tanks_num_shells = tanks_num_shells;
    }

    // === Distance Functions === //
    int AbstractGameBoardView::distance(const BoardCell &first, const BoardCell &second) const
    {
        return std::max(xDistance(first, second), yDistance(first, second));
    }

    int AbstractGameBoardView::xDistance(const BoardCell &first, const BoardCell &second) const
    {
        size_t dx = abs(first.getX() - second.getX());
        dx = std::min(dx, getWidth() - dx);
        return dx;
    }

    int AbstractGameBoardView::yDistance(const BoardCell &first, const BoardCell &second) const
    {
        size_t dy = abs(first.getY() - second.getY());
        dy = std::min(dy, getHeight() - dy);
        return dy;
    }

    // === Line, Direction and Neighbors Logic === //
    bool AbstractGameBoardView::isStraightLine(const BoardCell &from, const BoardCell &to) const
    {
        int x_diff = xDistance(from, to);
        int y_diff = yDistance(from, to);

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

    bool AbstractGameBoardView::isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir) const
    {
        if (!AbstractGameBoardView::isStraightLine(from, to))
        {
            return false;
        }

        BoardCell next_in_dir = getNextCellInDirection(from, dir);
        BoardCell next_in_straight = getNextCellInStraightLine(from, to);
        return next_in_dir == next_in_straight;
    }

    BoardCell AbstractGameBoardView::getNextCellInStraightLine(const BoardCell &from, const BoardCell &to) const
    {
        // assuming that from, to form a straight line in some direction.
        std::vector<BoardCell> neighbors = getAdjacentCells(from);
        int dist = distance(from, to);

        for (BoardCell next : neighbors)
        {
            std::pair<int, int> offsets = to - from;
            std::pair<int, int> next_offsets = to - next;
            int max_offset = std::max(std::abs(offsets.first), std::abs(offsets.second));
            std::pair<int,int> single_digit_offsets = {offsets.first / max_offset, offsets.second / max_offset};
            std::pair<int, int> fixed_next_offsets = {next_offsets.first + single_digit_offsets.first,
                                                      next_offsets.second + single_digit_offsets.second};
            int next_dist = this->distance(next, to);
            if (next_dist < dist && offsets == fixed_next_offsets)
            {
                return next;
            }
        }
        return from;
    }

    BoardCell AbstractGameBoardView::getNextCellInDirection(const BoardCell &c, const Direction dir) const
    {
        return this->createAdjustedBoardCell(c + dir);
    }

    std::vector<BoardCell> AbstractGameBoardView::getAdjacentCells(const BoardCell &curr_cell) const
    {
        std::vector<BoardCell> res;

        for (int dir_number = 0; dir_number < 8; dir_number++)
        {
            Direction dir = static_cast<Direction>(dir_number);
            BoardCell neighbor = this->createAdjustedBoardCell(curr_cell + dir);
            res.push_back(neighbor);
        }

        return res;
    }

    // === BoardCell Normalization === //
    BoardCell AbstractGameBoardView::createBoardCell(int x, int y) const
    {
        int w = static_cast<int>(getWidth());
        int h = static_cast<int>(getHeight());
        // The width, height parameters are signed int to prevent implicit conversion of the X,Y cell attributes to unsigned when performing % operation
        return BoardCell(((x % w) + w) % w, ((y % h) + h) % h);
    }

    BoardCell AbstractGameBoardView::createAdjustedBoardCell(const BoardCell &c) const
    {
        return createBoardCell(c.getX(), c.getY());
    }
}