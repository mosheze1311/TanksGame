#pragma once

#include "../GameObjects/GameObjectType.h"
#include "../GameBoard/BoardCell.h"

#include <unordered_set>
namespace UserCommon_211388913_322330820
{

    class GameBoardView
    {
    public:
        virtual ~GameBoardView() = default;

        // Returns a list of GameObjects at the given (row, col) cell
        virtual std::unordered_set<GameObjectType> getObjectsTypesOnCell(const BoardCell &c) const = 0;

        // Returns the width (number of columns) of the board
        virtual size_t getWidth() const = 0;

        // Returns the height (number of rows) of the board
        virtual size_t getHeight() const = 0;
    };
}