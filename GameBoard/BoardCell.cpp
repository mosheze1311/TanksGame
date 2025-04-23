#include "GameBoard.h"
#include "../GameObjects/GameObjects.h"

BoardCell::BoardCell():x(0), y(0) {};
BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (x < other.getX()) || (x == other.getX() && y < other.getY());
}
BoardCell BoardCell::operator+(Direction dir) const
{
    auto [dx, dy] = DirectionUtils::directionOffsets(dir);
    return BoardCell(x + dx, y + dy);
}
BoardCell BoardCell::operator-(Direction dir) const
{
    auto [dx, dy] = DirectionUtils::directionOffsets(dir);
    return BoardCell(x - dx, y - dy);
}
int BoardCell::getX() const
{
    return this->x;
}
int BoardCell::getY() const
{
    return this->y;
}

bool BoardCell::operator==(const BoardCell& other) const {
    return x == other.x && y == other.y;
}

// TODO: maybe move to board so it will use the dimensions to move between sides
int BoardCell::distance(BoardCell other){
    int x_distance = abs(this->getX() - other.getX());
    int y_distance = abs(this->getY() - other.getY());
    return max(x_distance, y_distance);
}