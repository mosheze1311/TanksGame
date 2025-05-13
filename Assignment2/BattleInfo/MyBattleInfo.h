#pragma once

#include "../common/BattleInfo.h"
#include "../common/SatelliteView.h"
#include "../GameBoard/GameBoard.h"
#include "../common/ActionRequest.h"
#include <vector>
#include <queue>
#include <optional>


// TODO: choose better name
class MyBattleInfo : public BattleInfo
{

private:
    // === Data sent from Player to TankAlgorithm === //
    SatelliteView &sat_view;

    BoardCell current_cell;
    size_t tank_index;
    size_t player_index;
    size_t remaining_steps;
    size_t remaining_shells;
    size_t board_rows;
    size_t board_cols;


public:
    // === Constructor === //
    MyBattleInfo(SatelliteView& sat_view, BoardCell cell, size_t tank_idx, size_t player_idx,
                 size_t steps, size_t shells, size_t rows, size_t cols)
        : current_cell(cell),
          tank_index(tank_idx),
          player_index(player_idx),
          remaining_steps(steps),
          remaining_shells(shells),
          sat_view(sat_view),
          board_rows(rows),
        board_cols(cols) {};

    // === Destructor === //
    ~MyBattleInfo() override = default;

    // === Getters === //
    BoardCell getCurrentCell() const { return current_cell; }
    size_t getTankIndex() const { return tank_index; }
    size_t getPlayerIndex() const { return player_index; }
    size_t getMaxSteps() const { return remaining_steps; }
    size_t getRemainingShells() const { return remaining_shells; }

    // === Setters === //
    void setCurrentCell(BoardCell new_cell)
    {
        current_cell = new_cell;
    }

    void setMaxSteps(size_t steps)
    {
        remaining_steps = steps;
    }

    void setRemainingShells(size_t shells)
    {
        remaining_shells = shells;
    }


    // === Additional methods (rename later) === //
// std::optional<BoardCell> findClosestTank(GameObjectType targetType) const {

//     std::queue<BoardCell> q;
//     std::set<BoardCell> visited;

//     q.push(current_cell);
//     visited.insert(current_cell);

//     while (!q.empty()) {
//         BoardCell cell = q.front();
//         q.pop();

//         char obj = sat_view.getObjectAt(cell.getX(), cell.getY());

//         // match the character against tank type
//         if ((targetType == GameObjectType::tank1 && obj == '1') ||
//             (targetType == GameObjectType::tank2 && obj == '2')) {
//             if (cell != current_cell) // skip self
//                 return cell;
//         }

//         // explore neighbors (up, down, left, right)
//         const std::vector<BoardCell> neighbors = {
//             BoardCell(cell.getX() + 1, cell.getY()),
//             BoardCell(cell.getX() - 1, cell.getY()),
//             BoardCell(cell.getX(), cell.getY() + 1),
//             BoardCell(cell.getX(), cell.getY() - 1),
//         };

//         for (const BoardCell& nb : neighbors) {
//             if (nb.getX() >= board_rows || nb.getY() >= board_cols)
//                 continue;
//             if (visited.count(nb))
//                 continue;
//             char ch = sat_view.getObjectAt(nb.getX(), nb.getY());
//             if (ch != '#' && ch != '@' && ch != '&') { // wall, mine, out of bounds
//                 q.push(nb);
//                 visited.insert(nb);
//             }
//         }
//     }

//     return std::nullopt; // not found
// }


// TODO: Rethink about functions that can be added here. 
};