#pragma once

#include "../common/TankAlgorithm.h"
#include "../GameBoard/GameBoard.h"
#include "../BattleInfo/MyBattleInfo.h"

// TODO: finish implementation
class MyTankAlgorithm : public TankAlgorithm
{

private:
    BoardCell current_cell;
    size_t remaining_shells;

public:
    // === Constructor === //
    MyTankAlgorithm();
    // === Destructor === //
    ~MyTankAlgorithm() override = default;

    ActionRequest getAction()
    {
    }
    
    void updateBattleInfo(BattleInfo &info) override
    {
        auto *current_info = dynamic_cast<MyBattleInfo *>(&info);
        if (!current_info)
        {
            // Should never happen
            return;
        }

        this->current_cell = current_info->getCurrentCell();
        this->remaining_shells = current_info->getRemainingShells();
    }
};