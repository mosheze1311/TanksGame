#pragma once

#include "../common/TankAlgorithm.h"

#include "TankAlgorithmUtils.h"

#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../SatelliteView/BoardSatelliteView.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"

#include <queue>
#include <set>

class AbstractTankAlgorithm : public TankAlgorithm
{
protected:
    // === Attributes ===
    size_t tank_idx;
    size_t player_idx;
    size_t num_of_shells;
    BoardCell assumed_location;
    Direction direction;
    size_t current_step;
    size_t shoot_cooldown;

public:
    // === Constructor ===
    AbstractTankAlgorithm(size_t tank_idx, size_t player_idx, size_t num_of_shells,
                          BoardCell assumed_location, Direction direction, size_t current_step);

protected:

    // === Cooldown Management ===
    bool canTankShoot() const;
    void execute_shoot();

    // === Step Logic ===
    void advance_step();

    // === Action Planning ===
    ActionRequest getTankEvasionAction(const SatelliteAnalyitcsView &sat_view, BoardCell chaser_location, Direction chaser_direction) const;
    ActionRequest getTankAggressiveAction(const SatelliteAnalyitcsView &sat_view) const;
    ActionRequest advanceTankToTarget(const SatelliteAnalyitcsView &sat_view, BoardCell target) const;
    ActionRequest adjustDirection(BoardCell to, size_t width, size_t height) const;

    // === Target Evaluation ===
    std::optional<ActionRequest> attemptShoot(BoardCell target, size_t width, size_t height) const;
    std::optional<ActionRequest> escapeShells(const SatelliteAnalyitcsView &sat_view) const;
    std::optional<BoardCell> getEscapingRoute(SatelliteAnalyitcsView sat_view, Direction enemy_dir) const;

    // === Pathfinding ===
    void Dijkstra(const SatelliteAnalyitcsView &sat_view, GameObjectType tank_type, BoardCell start, BoardCell target,
                  std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const;

    // === Target Estimation ===
    BoardCell approxClosestEnemyTankLocation(const SatelliteAnalyitcsView &sat_view) const;

    // === Setters === //
    void setCurrentLocation(const BoardCell& loc);
    void setRemainingShells(size_t shells);
    void setCurrentStep(size_t step);
    void setTankDirection(Direction dir);
    void setTankIndex(size_t idx);

    
    // === Getters ===
    size_t getRemainingShells() const;
    BoardCell getCurrentLocation() const;
    Direction getTankDirection() const;
    size_t getCurrentStep() const;
    size_t getShootCooldown() const;
    size_t getTankIdx() const;
    GameObjectType getTankType() const;
};