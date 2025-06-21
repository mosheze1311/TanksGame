#pragma once

#include "../common/SatelliteView.h"

#include "AssumedDirection.h"

#include "../GameBoard/BoardCell.h"
#include "../GameObjects/Direction.h"
#include "../GameBoardView/GameBoardView.h"
#include "../GameObjects/GameObjectType.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../Utils/DirectionUtils.h"
#include "../Utils/GameObjectTypeUtils.h"


#include <vector>
#include <map>

#define AnalyticalViewMap std::map<BoardCell, std::vector<std::pair<GameObjectType, AssumedDirection>>> // now allows multiple objects per cell

class SatelliteAnalyticsView : public GameBoardView
{
private:
    // === Attributes === //
    int player_idx;
    size_t max_steps_gap;

    size_t width, height;
    AnalyticalViewMap analytical_view;
    size_t last_updated_step = 0;

    std::set<BoardCell> shells_locations;
    std::set<BoardCell> enemies_locations;
    std::set<BoardCell> mines_locations;

    // === INIT Helpers === //
    BoardCell shellExpectedLocation(const BoardCell &old_location, AssumedDirection assumed_dir, size_t steps_gap) const;
    bool isDirectionMatchShell(const BoardCell &old_location, const BoardCell &new_location, AssumedDirection assumed_dir, size_t steps_gap) const;
    AssumedDirection findMatchingShellDirection(const BoardCell &old_location, size_t steps_gap) const;

    // === INIT === //
    void initAnalyticalView(const SatelliteView &sat_view);
    void clearView();

    // === Move Objects on View === //
    void advanceShells();
    void advanceShellsOnce();

    // === Internal Logic === //
    BoardCell createAdjustedCell(const BoardCell &c) const;
    std::vector<std::pair<char, AssumedDirection>> getObjectsAtInternal(const BoardCell &c) const;
    void addObject(const BoardCell &location, GameObjectType t, AssumedDirection d);
    void addObjectInternal(const BoardCell &location, GameObjectType obj_type, AssumedDirection dir);
    void removeObjectInternal(const BoardCell &old_shell_location, GameObjectType obj_type);

public:
    // === Constructor === //
    SatelliteAnalyticsView(size_t height, size_t width, size_t max_steps_gap, int player_idx);

    //=== Destructor ===//
    ~SatelliteAnalyticsView() override = default;

    //=== Getters ===//
    size_t getWidth() const override;
    size_t getHeight() const override;
    size_t getMaxStepGap() const;
    size_t getEnemyTanksNum() const;
    std::set<BoardCell> getEnemyTanksLocations() const;
    std::set<BoardCell> getShellsLocations() const;
    std::set<BoardCell> getMinesLocations() const;
    std::vector<std::pair<char, AssumedDirection>> getObjectsAt(const BoardCell &c) const;
    std::unordered_set<GameObjectType> getObjectsTypesOnCell(const BoardCell &c) const override;
    std::optional<AssumedDirection> getDirectionOfObjectAt(GameObjectType t, const BoardCell &c) const;
    std::optional<std::pair<GameObjectType, AssumedDirection>> getObjectAt(GameObjectType obj_type, const BoardCell &location) const;
    
    // === Movement validation === //
    bool isWallOnCell (const BoardCell &cell) const;

    // === Update View Functions === //
    void updateAnalyticalView(const SatelliteView &sat_view, size_t current_step);
    void updateShellsDirectionsFromView(const SatelliteAnalyticsView &other);

    void applyApproxBoardChanges(); // this function should be called at the beginning of each step to update the view
    void addShell(const BoardCell &location, Direction dir);
};
