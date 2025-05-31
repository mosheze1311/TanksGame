#include "BoardCell.h"

#include "../Utils/DirectionUtils.h"

// === Constructors === //
BoardCell::BoardCell() : x(0), y(0) {};

BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// === Getters === //
int BoardCell::getX() const
{
    return this->x;
}

int BoardCell::getY() const
{
    return this->y;
}

// === Overloading Comparison Operators === //
// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (y < other.getY()) || (y == other.getY() && x < other.getX()); // we compare in this order so the output logging will match the requirements (rows before cols)
}

// === Overloading Arithmetic Operators === //
BoardCell BoardCell::operator+(const std::pair<int, int> &other) const
{
    auto [dx, dy] = other;
    return BoardCell(x + dx, y + dy);
}

BoardCell BoardCell::operator+(Direction dir) const
{
    return *this + DirectionUtils::directionOffsets(dir);
}

BoardCell BoardCell::operator-(const std::pair<int, int> &other) const
{
    auto [dx, dy] = other;
    return BoardCell(x - dx, y - dy);
}

BoardCell BoardCell::operator-(Direction dir) const
{
    return *this - DirectionUtils::directionOffsets(dir);
}

// === Overloading << Operator for Printing === //
std::ostream &operator<<(std::ostream &os, const BoardCell &cell)
{
    return os << "(" << cell.getX() << ", " << cell.getY() << ")";
}