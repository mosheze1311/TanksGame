#pragma once

#include "BoardCell.h"

#include "../AbstractGameBoardView/AbstractGameBoardView.h"

#include "../../common/SatelliteView.h"
#include "../GameObjects/GameObjects.h"

#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace UserCommon_211388913_322330820
{
    // Forward declarations
    class GameObject;

    // Classes declarations
    class GameBoard : public AbstractGameBoardView
    {
    private:
        std::unordered_map<GameObject *, BoardCell> objects_locations;
        std::map<BoardCell, std::unordered_set<GameObject *>> board;

        // ownership map
        std::unordered_map<GameObject *, std::unique_ptr<GameObject>> owned_objects;

        // === Functions === //
        std::unique_ptr<GameObject> createGameObjectOfType(GameObjectType type);

        // Adds an object to the board - internal use, arguments are treated as valid
        void addObjectInternal(std::unique_ptr<GameObject> obj, const BoardCell &c);

        // Removes an object from the board - internal use, arguments are treated as valid
        void removeObjectInternal(GameObject *obj, bool final = true);

        // Places the object on the requested location - Assumes object is owned by board and not placed elsewhere
        void placeObjectOnBoard(GameObject *obj, const BoardCell &c);

        // Updates the count of a specific object type on the board by adding incremental
        void updateObjectCount(const GameObject *obj, int incremental);

        // === Modify Board Functions === //
        // Add an object to the board on the requested cell
        void addObject(std::unique_ptr<GameObject> obj, const BoardCell &c);

        // tank API to inform that a shell was shot and the board need to update it's remaining shell count
        void useTankShell();

    public:
        // === Constructor === //
        GameBoard();
        
        // === Destructor === //
        ~GameBoard() override = default;

        // === Copy Constructor (DELETED) === //
        GameBoard(const GameBoard &board) = delete;
        // === Assignment Operator (DELETED) === //
        GameBoard &operator=(const GameBoard &other) = delete;

        // === Move Constructor (DELETED) === //
        GameBoard(GameBoard &&board) = delete;
        //  === Move Assignment Operator (DELETED) === //
        GameBoard &operator=(GameBoard &&other) = delete;

        // === Init Board === //
        void initFromDetails(const SatelliteView &sat_view, size_t width, size_t height, size_t max_steps, size_t num_shells);

        // === Override Getters === //
        // get all cells with objects on them.
        std::vector<BoardCell> getOccupiedCells() const override;

        // get a set of all objects on the cell
        std::unordered_set<GameObjectType> getObjectsTypesOnCell(const BoardCell &c) const override;

        // === Getters === //
        // get a set of all objects on the cell
        std::unordered_set<GameObject *> getObjectsOnCell(const BoardCell &c) const;

        // get all objects of a certain GameObjectType that exist on board
        std::vector<GameObject *> getGameObjects(GameObjectType t) const;

        // get all objects on the board
        std::vector<GameObject *> getAllGameObjects() const;

        // get all tanks in sacial order (left to right, up to down)
        std::vector<Tank *> getAllTanksOrderedByCell() const;

        // get all tanks for a player
        std::vector<Tank *> getTanks(GameObjectType t) const;

        // get an optional cell location of an object on board. if object not on board, return nullopt.
        std::optional<BoardCell> getObjectLocation(const GameObject *go) const;

        // === Board State Functions === //
        // Check if there is any object on the cell
        bool isOccupiedCell(const BoardCell &c) const;

        // Check if the specific object exists on board
        bool isObjectOnBoard(const GameObject *obj) const;

        // === Modify Board Functions === //
        // moves an object on the board from its current cell to new position. ignores if object not on board
        // this is used by moving objects to handle their movements - keep public to allow access to moving objects
        void moveGameObject(GameObject *obj, const BoardCell &new_position);

        // remove the object from the board
        // used by all objects in death
        void removeObject(GameObject *obj);

        void addTankShell(std::unique_ptr<Shell> shell, const BoardCell &shell_initial_cell);
    };
}