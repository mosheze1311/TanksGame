#pragma once

#include "../BattleInfo/BattleInfoAgent.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include "../../common/TankAlgorithm.h"

#include "../../UserCommon/Config/ConfigReader.h"
#include "../../UserCommon/BoardSatelliteView/BoardSatelliteView.h"
#include "../../UserCommon/GameCollisionHandler/GameCollisionHandler.h"
#include "../../UserCommon/Utils/ActionRequestUtils.h"
#include "../../UserCommon/Utils/TankCooldownHandler.h"
#include "../../UserCommon/Utils/GameObjectTypeUtils.h"

#include <queue>
#include <set>
namespace Algorithm_211388913_322330820
{
    using namespace UserCommon_211388913_322330820;
    class AbstractTankAlgorithm : public TankAlgorithm
    {
    protected:
        // === Attributes === //
        int tank_idx;
        int player_idx;

        BoardCell assumed_location;
        Direction direction;

        size_t num_of_shells = 0;
        CooldownHandler cooldowns;

        size_t current_step = 0; // this means that actual first step is 1
        size_t max_steps = 0;

        size_t step_to_get_info = 1; // get info on first step

        // these parameters are initiated on the first GetBattleInfo
        SatelliteAnalyticsView sat_view;
    protected:
        // Determines the tank's next action based on the current game state.
        // Must be implemented by derived classes to define tank behavior logic.
        // Called internally by getAction(), which also handles state adjustments before and after getActionLogic().
        virtual ActionRequest getActionLogic() const = 0;

        // === Adjusting self parameters to the requested action === //
        void adjustSelfToAction(ActionRequest action);
        void adjustToForward();
        void adjustToBackward();
        void adjustToRotation(ActionRequest rotation);
        void adjustToShoot();
        void adjustToGetBattleInfo();

        // === Manage Shooting === //
        bool canTankShoot() const;
        bool hasShells() const;

        // === Step Logic === //
        void advanceStep();
        void handlePendingBackwards();

        // === Action Planning === //
        ActionRequest getTankEvasionAction(Direction chaser_direction) const;
        ActionRequest advanceTankToTarget(const BoardCell &target) const;
        ActionRequest adjustDirection(const BoardCell &to) const;

        // === Target Evaluation === //
        std::optional<ActionRequest> evaluateShootingOpportunity(const BoardCell &target) const;
        std::optional<ActionRequest> escapeShells() const;
        std::optional<BoardCell> getEscapingRoute(Direction enemy_dir) const;

        // === Pathfinding === //
        std::optional<BoardCell> findNextCellInPath(const BoardCell &start, const BoardCell &target) const;
        void Dijkstra(const BoardCell &start, const BoardCell &target,const std::set<BoardCell> & forbidden ,std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const;
        std::set<BoardCell> getAlliesReservedCells() const;

            // === Target Estimation === //
        BoardCell approxClosestEnemyTankLocation() const;

        // === Setters === //
        void setCurrentLocation(const BoardCell &loc);
        void setRemainingShells(size_t shells);
        void setCurrentStep(size_t step);
        void setTankDirection(Direction dir);
        void setTankIndex(size_t idx);
        void setMaxSteps(size_t max_steps);

        // === Getters === //
        int getBoardWidth() const;
        int getBoardHeight() const;
        size_t getRemainingShells() const;
        BoardCell getCurrentLocation() const;
        Direction getTankDirection() const;
        size_t getCurrentStep() const;
        size_t getTankIdx() const;
        GameObjectType getTankType() const;

        bool canMoveToCell(const BoardCell &cell) const;

        // === Manage Shooting Range === //
        bool isShellInTankDirection(const BoardCell &shell_loc, AssumedDirection shell_assumed_dir) const;
        bool inShootRange(const BoardCell &to) const;

        // === Survival Tactic === //
        static std::pair<ActionRequest, float> getBestProbSurvivalAction(const AbstractTankAlgorithm &algo, int steps_to_calculate, ActionRequest default_action);
        static float bestScoreFor(const AbstractTankAlgorithm &algo, ActionRequest action, int steps_to_calculate);
        bool isShellApproaching() const;


        // === Aggresive Algorithm === //
        ActionRequest getTankAggressiveAction() const;



    public:
        // === Constructor === //
        AbstractTankAlgorithm(int tank_idx, int player_idx);

        // === Destructor === //
        virtual ~AbstractTankAlgorithm() override = default;

        // === clone === //
        virtual std::unique_ptr<AbstractTankAlgorithm> clone() const = 0;

        // === Interface Implementation === //
        ActionRequest getAction() override;
        void updateBattleInfo(BattleInfo &info) override;
    };

}