#pragma once

#include "../GameObjects/Direction.h"

#include <ostream>
#include <utility>
namespace UserCommon_211388913_322330820
{

    // Forward declarations
    enum class Direction;

    class BoardCell
    {
    private:
        // === Atributes === //
        // values are not const to allow assignment using operator=. cannot really change them - no setters.
        int x;
        int y;

    public:
        // === Constructors === //
        // Empty constructor - do not delete, used in STL containers.
        BoardCell();

        BoardCell(int x, int y);

        // === Getters === //
        int getX() const;

        int getY() const;

        // === Overloading Comparison Operators === //
        // Allow using BoardCell as a key in a map
        bool operator<(const BoardCell &other) const;
        
        // Allow comparing cells (default compares all fields by value)
        bool operator==(const BoardCell &other) const = default;

        // === Overloading Arithmetic Operators === //
        // Returns a new cell moved in the given direction
        BoardCell operator+(Direction dir) const;

        // Returns a new cell moved opposite to the given direction
        BoardCell operator-(Direction dir) const;

        // Returns a new cell offset by the given delta
        BoardCell operator+(const std::pair<int, int> &offsets) const;

        // Returns a new cell offset negatively by the given delta
        BoardCell operator-(const std::pair<int, int> &offsets) const;

        // Returns the offset delta of the cells
        std::pair<int, int> operator-(const BoardCell &other) const;
    };

    // === Overloading << Operator for Printing === //
    std::ostream &operator<<(std::ostream &os, const BoardCell &cell);
}