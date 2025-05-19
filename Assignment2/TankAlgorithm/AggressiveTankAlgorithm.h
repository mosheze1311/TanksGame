#pragma once

#include "../common/TankAlgorithm.h"
#include "../GameBoard/GameBoard.h"
#include "../BattleInfo/BattleInfoAgent.h"
#include "AbstractTankAlgorithm.h"

// TODO: finish implementation
class AggressiveTankAlgorithm : public AbstractTankAlgorithm
{

private:
    // === Attributes === //
    int player_index;
    int tank_index;
    BoardCell current_cell;
    size_t remaining_shells;

    int getTankIndex() const
    {
        return tank_index;
    }
    
public:
    
    // === Constructor === //
    AggressiveTankAlgorithm(int player_index, int tank_index)
        : player_index(player_index),
          tank_index(tank_index),
          current_cell(0, 0),
          remaining_shells(0) {};
    
    // === Destructor === //
    ~AggressiveTankAlgorithm() override = default;


    // === Public Interface === //
    ActionRequest getAction()
    {
    }

    void updateBattleInfo(BattleInfo &info) override
    {
        auto *current_info = dynamic_cast<BattleInfoAgent *>(&info);
        if (!current_info)
        {
            // Should never happen
            return;
        }

        this->current_cell = current_info->getCurrentCell();
        this->remaining_shells = current_info->getRemainingShells();
    }
};