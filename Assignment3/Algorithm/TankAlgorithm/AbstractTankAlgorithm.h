#pragma once

#include "../Config/ConfigReader.h"

#include "../common/TankAlgorithm.h"

#include "../BattleInfo/BattleInfoAgent.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
#include "../SatelliteAnalyticsView/SatelliteAnalyticsView.h"
#include "../Utils/TankCooldownHandler.h"
#include "../Utils/GameObjectTypeUtils.h"

#include <queue>
#include <set>

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
    int width = 0;
    int height = 0;

protected:
    // Determines the tank's next action based on the current game state.
    // Must be implemented by derived classes to define tank behavior logic.
    // Called internally by getAction(), which also handles state adjustments before and after getActionLogic().
    virtual ActionRequest getActionLogic() const = 0;

    void adjustSelfToAction(ActionRequest action);

    // === Manage Shooting === //
    bool canTankShoot() const;
    bool hasShells() const;
    void executeShoot();

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
    void Dijkstra(GameObjectType tank_type, const BoardCell &start, const BoardCell &target,
                  std::map<BoardCell, int> &distances, std::map<BoardCell, BoardCell> &parents) const;

    // === Target Estimation === //
    BoardCell approxClosestEnemyTankLocation() const;

    // === Setters === //
    void setCurrentLocation(const BoardCell &loc);
    void setRemainingShells(size_t shells);
    void setCurrentStep(size_t step);
    void setTankDirection(Direction dir);
    void setTankIndex(size_t idx);
    void setMaxSteps(size_t max_steps);
    void setBoardHeight(int height);
    void setBoardWidth(int width);

    // === Getters === //
    int getBoardWidth() const;
    int getBoardHeight() const;
    size_t getRemainingShells() const;
    BoardCell getCurrentLocation() const;
    Direction getTankDirection() const;
    size_t getCurrentStep() const;
    size_t getShootCooldown() const;
    size_t getTankIdx() const;
    GameObjectType getTankType() const;

    bool canMoveToCell(const BoardCell &cell) const;

    // === Manage Shooting Range === //
    bool isShellChasingTank(const BoardCell &shell_loc, AssumedDirection shell_assumed_dir) const;
    bool inShootRange(const BoardCell &to) const;

    // === Wrappers for GameBoardUtils Functions === //
    int distance(const BoardCell &from, const BoardCell &to) const;
    BoardCell createAdjustedBoardCell(const BoardCell &c) const;
    BoardCell getNextCellInDirection(const BoardCell &c, Direction dir) const;
    BoardCell getNextCellInStraightLine(const BoardCell& from, const BoardCell& to) const;  
    std::vector<BoardCell> getAdjacentCells(const BoardCell &cell) const;
    bool isDirectionMatch(const BoardCell &from, const BoardCell &to, Direction dir) const;
    bool isStraightLine(const BoardCell &from, const BoardCell &to) const;

public:
    // === Constructor === //
    AbstractTankAlgorithm(int tank_idx, int player_idx);

    // === Destructor === //
    virtual ~AbstractTankAlgorithm() override = default;

    // === Interface Implementation === //
    ActionRequest getAction() override;
    void updateBattleInfo(BattleInfo &info) override;
};