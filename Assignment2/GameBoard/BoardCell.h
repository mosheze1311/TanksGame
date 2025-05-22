#pragma once

#include "../GameObjects/Direction.h"

#include <ostream>
#include <utility>

// Forward declarations
enum class Direction;


class BoardCell
{
private:
    // === Atributes ===
    // values are not const to allow assignment using operator=. cannot really change them - no setters.
    int x;
    int y;

public:
    // === Constructors ===
    // Empty constructor - do not delete, used in STL containers.
    BoardCell();

    BoardCell(int x, int y);

    // === Getters ===
    [[nodiscard]] int getX() const;

    [[nodiscard]] int getY() const;

    // === Overloading Comparison Operators ===
    // Allow using BoardCell as a key in a map
    [[nodiscard]] bool operator<(const BoardCell &other) const;

    // Allow comparing cells (default compares all fields by value)
    [[nodiscard]] bool operator==(const BoardCell &other) const = default;

    // === Overloading Arithmetic Operators ===
    // Returns a new cell moved in the given direction
    [[nodiscard]] BoardCell operator+(Direction dir) const;
    
    // Returns a new cell moved opposite to the given direction
    [[nodiscard]] BoardCell operator-(Direction dir) const;

    // Returns a new cell offset by the given delta
    [[nodiscard]] BoardCell operator+(const std::pair<int, int> &offsets) const;

    // Returns a new cell offset negatively by the given delta
    [[nodiscard]] BoardCell operator-(const std::pair<int, int> &offsets) const;

};

// === Overloading << Operator for Printing ===
std::ostream &operator<<(std::ostream &os, const BoardCell &cell);
