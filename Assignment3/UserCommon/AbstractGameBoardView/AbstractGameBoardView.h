#pragma once

#include "../GameBoard/BoardCell.h"
#include "../GameObjects/GameObjectType.h"

#include <vector>
#include <unordered_set>
#include <map>

namespace UserCommon_211388913_322330820
{

    class AbstractGameBoardView
    {
    protected:
        // === Nested class to store board details === //
        class BoardDetails
        {
        public:
            // === Attributes === //
            // board properties
            size_t width;
            size_t height;
            size_t max_steps;
            size_t tanks_num_shells;

            // objects counters
            size_t walls;
            size_t mines;
            size_t shells;
            std::map<GameObjectType, size_t> tanks_count;

            // remaining shells to fire (for concluding game)
            size_t remaining_shells;

            // === Constructor === //
            BoardDetails(size_t width, size_t height, size_t max_steps, size_t num_shells);
        };

        // === Attributes === //
        BoardDetails board_details;

        // === Setters === //
        void setWidth(size_t width);
        void setHeight(size_t height);
        void setMaxSteps(size_t max_steps);
        void setTanksNumShells(size_t tanks_num_shells);

    public:
        AbstractGameBoardView(size_t width, size_t height, size_t max_steps, size_t num_shells);
        virtual ~AbstractGameBoardView() = default;

        // === Getters === //
        size_t getWidth() const;
        size_t getHeight() const;
        size_t getMaxSteps() const;
        size_t getTanksNumShells() const;
        std::map<GameObjectType, size_t> getTanksCountPerType() const;
        int getGameObjectCount(GameObjectType type) const;
        int getTotalRemainingShells() const;


        // Returns a list of GameObjects at the given (row, col) cell
        virtual std::unordered_set<GameObjectType> getObjectsTypesOnCell(const BoardCell &c) const = 0;

        virtual std::vector<BoardCell> getOccupiedCells() const = 0;

        // === Distance Functions === //
        // get distance of 2 cells
        int distance(const BoardCell &first, const BoardCell &second) const;

        // finds x distance
        int xDistance(const BoardCell &first, const BoardCell &second) const;

        // finds y distance
        int yDistance(const BoardCell &first, const BoardCell &second) const;

        std::tuple<int, int, int, int> proximityBoundaries(const BoardCell &center, int proximity) const;

        // === Line, Direction and Neighbors Logic === //
        bool isStraightLine(const BoardCell &from, const BoardCell &to) const;

        bool isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir) const;

        BoardCell getNextCellInStraightLine(const BoardCell &from, const BoardCell &to) const;

        BoardCell getNextCellInDirection(const BoardCell &c, Direction dir) const;

        // get neigbhor cells
        std::vector<BoardCell> getAdjacentCells(const BoardCell &curr_cell) const;

        // === BoardCell Normalization === //
        BoardCell createBoardCell(int x, int y) const;
        BoardCell createAdjustedBoardCell(const BoardCell &c) const ;
    };
}