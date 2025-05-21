#pragma once
#include "../GameObjects/Direction.h"


// Forward declarations
enum class Direction;


class BoardCell
{
private:
    int x;
    int y;

public:
    // Empty constructor - do not delete
    BoardCell();

    // Constructor
    BoardCell(int x, int y);

    // Overload < operator for using BoardCell as a key in a map
    bool operator<(const BoardCell &other) const;

    // Overload + to add a Direction
    BoardCell operator+(const Direction dir) const;

    BoardCell operator+(const std::pair<int, int> &other) const;

    // Overload - to subtract a Direction
    BoardCell operator-(const Direction dir) const;

    BoardCell operator-(const pair<int, int> &other) const;

    // == operator
    bool operator==(const BoardCell &other) const;

    // Getters
    int getX() const;
    int getY() const;
};
