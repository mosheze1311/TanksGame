#pragma once

#include "AbstractTankAlgorithm.h"
#include "../Tactics/Tactic.h"

namespace Algorithm_211388913_322330820
{
    class CompetitionTankAlgorithm : public AbstractTankAlgorithm
    {
        friend class Bait;
        friend class Camper;
        friend class ScaredWolf;
        friend class Attacker;

    private:
        // === Attributes === //
        std::unique_ptr<Tactic> tactic;
        Formation formation;

    protected:
        
        // === Protected Interface Implementation === //
        ActionRequest getActionLogic() const override;

    public:
        // === Constructor === //
        CompetitionTankAlgorithm(int player_index, int tank_index);

        // === Destructor === //
        ~CompetitionTankAlgorithm() = default;
        
        // === Copy === //
        CompetitionTankAlgorithm(const CompetitionTankAlgorithm &other);
        CompetitionTankAlgorithm& operator=(const CompetitionTankAlgorithm &other);
        
        // === Move === //
        CompetitionTankAlgorithm(CompetitionTankAlgorithm &&other) = delete;
        CompetitionTankAlgorithm& operator=(CompetitionTankAlgorithm &&other) = delete;

        // === Setters === //
        void setTactic(std::unique_ptr<Tactic> new_tactic);
        void setFormation(Formation new_formation);

        // === Tactics === //
        std::unique_ptr<Tactic> decideBestTactic() const;
        std::unique_ptr<Tactic> getTactic() const;

        // === Public Interface Implementation === //
        std::unique_ptr<AbstractTankAlgorithm> clone() const override;
        void updateBattleInfo(BattleInfo& battle_info) override;

        BoardCell getBestEnemyLocation() const;     
    };
}