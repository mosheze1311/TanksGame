#include "CompetitionTankAlgorithm.h"

namespace Algorithm_211388913_322330820
{
    // === Constructor === //
    CompetitionTankAlgorithm::CompetitionTankAlgorithm(int player_index, int tank_index)
        : AbstractTankAlgorithm(
              tank_index,  // tank_idx
              player_index // player_idx
          )
    {
    }
    
    // === Copy Constructor & Operator === //
    CompetitionTankAlgorithm::CompetitionTankAlgorithm(const CompetitionTankAlgorithm& other)
        : AbstractTankAlgorithm(other)
    {
        if (other.tactic != nullptr)
        {
            this->tactic = other.tactic->clone();
        }
    }

    CompetitionTankAlgorithm& CompetitionTankAlgorithm::operator=(const CompetitionTankAlgorithm& other){
        if (this == &other)
            return *this;

        this->AbstractTankAlgorithm::operator=(other);
        if (other.tactic != nullptr){
            this->tactic = other.tactic->clone();
        }
        
        return *this;
    }

    // === Getters === //
    std::unique_ptr<Tactic> CompetitionTankAlgorithm::getTactic() const
    {
        return this->tactic->clone();
    };
    ActionRequest CompetitionTankAlgorithm::getActionLogic() const
    {

        ActionRequest default_action = ActionRequest::GetBattleInfo;
        // get battle info on required steps
        if (this->getCurrentStep() == this->step_to_get_info)
        {
            default_action = ActionRequest::GetBattleInfo;
        }
        else if (tactic){
            ActionRequest tactic_action =  tactic->getNextTacticAction(*this);
            default_action = tactic_action;
        }
        ActionRequest res = default_action;
        if (isShellApproaching())
        {
            res = getBestProbSurvivalAction(*this, 3, default_action).first;
        }
        return res;
    }

    // TODO: maybe this should be in abstract
    bool CompetitionTankAlgorithm::isShellApproaching() const
    {
        int step_dist = 5;
        step_dist = std::min(std::min(getBoardHeight(), getBoardWidth()), step_dist); // for small fields.
        int proximitiy = step_dist * ConfigReader::getConfig().getShellsSpeed();

        int left = this->assumed_location.getX() - proximitiy;
        int right = this->assumed_location.getX() + proximitiy;
        int up = this->assumed_location.getY() - proximitiy;
        int down = this->assumed_location.getY() + proximitiy;

        for (int x = left; x <= right; ++x)
        {
            for (int y = up; y <= down; ++y)
            {
                BoardCell cell = sat_view.createAdjustedBoardCell(BoardCell(x, y));
                if (this->sat_view.getObjectsTypesOnCell(cell).contains(GameObjectType::SHELL))
                {
                    AssumedDirection shell_direction = *(sat_view.getDirectionOfObjectAt(GameObjectType::SHELL, cell));
                    
                    if (shell_direction == AssumedDirection::UNKNOWN)
                        return true;
                    
                    int distance = sat_view.distance(cell, this->assumed_location);
                    BoardCell next_shell_location = sat_view.createAdjustedBoardCell(cell + static_cast<Direction>(shell_direction));
                    int next_distance = sat_view.distance(next_shell_location, this->assumed_location);
                    if (distance > next_distance)
                        return true;
                }
            }
        }
        return false;
    }

    std::unique_ptr<AbstractTankAlgorithm> CompetitionTankAlgorithm::clone() const
    {
        return std::make_unique<CompetitionTankAlgorithm>(*this);
    }

    // === Setters === //
    void CompetitionTankAlgorithm::setTactic(std::unique_ptr<Tactic> new_tactic)
    {
        this->tactic = std::move(new_tactic);
    }

    void CompetitionTankAlgorithm::setFormation(Formation new_formation)
    {
        this->formation = new_formation;
    };

    void CompetitionTankAlgorithm::updateBattleInfo(BattleInfo &battle_info)
    {
        this->AbstractTankAlgorithm::updateBattleInfo(battle_info); // call to parent function.

        BattleInfoAgent& current_info = dynamic_cast<BattleInfoAgent &>(battle_info);
        this->setFormation(current_info.getFormation());
        this->setTactic(decideBestTactic());
        current_info.setTactic(this->getTactic());
    }

    BoardCell CompetitionTankAlgorithm::getBestEnemyLocation() const{
        BoardCell formation_center = formation.getFormationCenter();
        BoardCell closest_enemy = {0, 0};
        int closest_dist = std::numeric_limits<int>::max();
        for (const BoardCell& enemy_loc: this->sat_view.getEnemyTanksLocations()){
            int dist = this->sat_view.distance(formation_center,  enemy_loc);
            if (dist < closest_dist){
                closest_dist = dist;
                closest_enemy = enemy_loc;
            }
        }
        return closest_enemy;

    }
    
    std::unique_ptr<Tactic> CompetitionTankAlgorithm::decideBestTactic() const
    {
        if (/*this->formation.size() == 1 && */this->getRemainingShells() == 0)
        {
            return std::make_unique<ScaredWolf>();
        } // if tank has no shells and alone in formation, scared wolf

        BoardCell enemy_location = getBestEnemyLocation();
        // int diffense_min_diff = -3;
        // if (this->sat_view.getTankDifference() <= diffense_min_diff)
        // { // player has much less tanks than opponent, scared wolf/camper
        //     if (this->getRemainingShells() == 0)
        //     {
        //         std::cout << "ScaredWolf tactic" << std::endl;
        //         return std::make_unique<ScaredWolf>();
        //     }
        //     std::cout << "Camper tactic" << std::endl;
        //     return std::make_unique<Camper>(this->formation.getFormationCenter(), enemy_location);
        // }

        // default - attack
        return std::make_unique<Attacker>(enemy_location);
    }
}
