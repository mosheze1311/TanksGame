#include "BoardCell.h"

using std::pair;
// === Constructors ===
BoardCell::BoardCell():x(0), y(0) {};

BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// === Operators ===
// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (x < other.getX()) || (x == other.getX() && y < other.getY());
}


BoardCell BoardCell::operator+(const pair<int, int>& other) const
{
    auto [dx, dy] = other;
    return BoardCell(x + dx, y + dy);
}

BoardCell BoardCell::operator+(Direction dir) const
{
    return *this + DirectionUtils::directionOffsets(dir);
}

BoardCell BoardCell::operator-(const pair<int, int> &other) const
{
    auto [dx, dy] = other;
    return BoardCell(x - dx, y - dy);
}

BoardCell BoardCell::operator-(Direction dir) const
{
    return *this - DirectionUtils::directionOffsets(dir);
}

bool BoardCell::operator==(const BoardCell& other) const {
    return x == other.x && y == other.y;
}

// === Getters ===
int BoardCell::getX() const
{
    return this->x;
}

int BoardCell::getY() const
{
    return this->y;
}
