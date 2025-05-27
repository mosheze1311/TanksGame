#pragma once
#include "../config.h"
#include "../common/TankAlgorithm.h"

#include "TankAlgorithmUtils.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include "../BattleInfo/BattleInfoAgent.h"
#include "../Utils/TankCooldownHandler.h"

#include <queue>
#include <set>

class AbstractTankAlgorithm : public TankAlgorithm
{
protected:
    // === Attributes ===
    size_t tank_idx;
    size_t player_idx;

    BoardCell assumed_location;
    Direction direction;
    size_t num_of_shells = 0;
    CooldownHandler cooldowns; 

    size_t current_step = 0; // this means that actual first step is 1
    size_t max_steps = 0;

    size_t step_to_get_info = 1; // get info on first step

    SatelliteAnalyticsView sat_view;

public:
    // === Constructor ===
    AbstractTankAlgorithm(size_t tank_idx, size_t player_idx);

    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo &info) override;

protected:
    virtual ActionRequest getActionLogic() = 0;
    void adjustSelfToAction(ActionRequest action);

    // === Cooldown / Wait Management ===
    void tickShootCooldown();
    void tickBackwardsWait();
    bool isPendingBackwards() const;
    bool canImmediateBackwards() const;
    void extendBackwardsStreak();
    void cancelBackwardsWait();
    void startBackwardsWait();
    bool canTankShoot() const;

    bool hasShells() const;
    void executeShoot();

    // === Step Logic ===
    void advanceStep();
    void handlePendingBackwards();

    // === Action Planning ===
    ActionRequest getTankEvasionAction(const SatelliteAnalyticsView &sat_view, Direction chaser_direction) const;
    ActionRequest getTankAggressiveAction(const SatelliteAnalyticsView &sat_view) const;
    ActionRequest advanceTankToTarget(const SatelliteAnalyticsView &sat_view, BoardCell target) const;
    ActionRequest adjustDirection(BoardCell to, size_t width, size_t height) const;

    // === Target Evaluation ===
    std::optional<ActionRequest> evaluateShootingOpportunity(BoardCell target, size_t width, size_t height) const;
    std::optional<ActionRequest> escapeShells(const SatelliteAnalyticsView &sat_view) const;
    std::optional<BoardCell> getEscapingRoute(SatelliteAnalyticsView sat_view, Direction enemy_dir) const;

    // === Pathfinding ===
    void Dijkstra(const SatelliteAnalyticsView &sat_view, GameObjectType tank_type, BoardCell start, BoardCell target,
                  std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const;

    // === Target Estimation ===
    BoardCell approxClosestEnemyTankLocation(const SatelliteAnalyticsView &sat_view) const;

    // === Setters === //
    void setCurrentLocation(const BoardCell &loc);
    void setRemainingShells(size_t shells);
    void setCurrentStep(size_t step);
    void setTankDirection(Direction dir);
    void setTankIndex(size_t idx);
    void setMaxSteps(size_t max_steps);

    // === Getters ===
    size_t getRemainingShells() const;
    BoardCell getCurrentLocation() const;
    Direction getTankDirection() const;
    size_t getCurrentStep() const;
    size_t getShootCooldown() const;
    size_t getTankIdx() const;
    GameObjectType getTankType() const;

    bool canMoveToCell (const BoardCell &cell) const;
};