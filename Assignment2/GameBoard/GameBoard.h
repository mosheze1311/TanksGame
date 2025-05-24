#pragma once

#include "../GameObjects/GameObjects.h"
#include "../GameBoardInitializer/GameBoardInitializer.h"
#include "BoardCell.h"

#include <map>
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Forward declarations
class GameObject;
class GameBoardInitializer;
// TODO: probably need the forward declaration of GameBoardInitializer for the initFromFile impl - check

// Classes declarations

class GameBoard
{
private:
    // === Nested class to store board details ===
    class BoardDetails
    {
    public:
        // === Attributes ===
        // board properties
        size_t height;
        size_t width;
        size_t max_steps;
        size_t tanks_num_shells;

        // objects counters
        size_t walls;
        size_t mines;
        size_t shells;
        std::map<GameObjectType, size_t> tanks_count;

        // remaining shells to fire (for concluding game)
        size_t remaining_shells;

        // === Constructor ===
        BoardDetails(int height, int width);
    };

    // === Attributes ===
    BoardDetails board_details;
    std::unordered_map<GameObject *, BoardCell> objects_locations;
    std::map<BoardCell, std::unordered_set<GameObject *>> board;

    // ownership map
    std::unordered_map<GameObject *, std::unique_ptr<GameObject>> owned_objects;


    // === Functions ===
    // create a board cell that fits the board without overflowing
    BoardCell createBoardCell(int x, int y) const;

    // create a board cell that fits the board without overflowing
    BoardCell createAdjustedBoardCell(const BoardCell &c) const;

    // Adds an object to the board - internal use, arguments are treated as valid
    void addObjectInternal(std::unique_ptr<GameObject> obj, const BoardCell &c);

    // Removes an object from the board - internal use, arguments are treated as valid
    void removeObjectInternal(GameObject *obj, bool final = true);

    // Places the object on the requested location - Assumes object is owned by board and not placed elsewhere
    void placeObjectOnBoard(GameObject *obj, const BoardCell &c);

    // Updates the count of a specific object type on the board by adding incremental
    void updateObjectCount(const GameObject *obj, int incremental);

    // === Setters ===
    void setWidth(size_t width);

    void setHeight(size_t height);

    void setMaxSteps(size_t max_steps);

    void setTanksNumShells(size_t tanks_num_shells);

public:
    // Constructor
    GameBoard(int height, int width);

    // Copy Constructor (DELETED)
    GameBoard(const GameBoard &board) = delete;

    ~GameBoard();

    // Assignment Operator (DELETED)
    GameBoard &operator=(const GameBoard &other) = delete;

    // TODO: do we need Rule of 5?

    // === Initiate from File ===
    bool initFromFile(const std::string &input_file_path);

    //=== Getters ===
    size_t getWidth() const;

    size_t getHeight() const;

    size_t getMaxSteps() const;

    size_t getTanksNumShells() const;

    std::map<GameObjectType, size_t> getTanksCountPerType() const;

    // get count of object type on board
    int getGameObjectCount(GameObjectType type) const;

    // get the total remaining shells count
    int getTotalRemainingShells() const;

    // get all cells with objects on them.
    std::vector<BoardCell> getOccupiedCells() const;

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

    //=== Modify Board Functions ===
    // Add an object to the board on the requested cell
    // TODO: Tank uses this to add shells to game. do we need it to be public or simply allow tank to access it somehow? maybe just add a request shoot that will do it instead of tank?
    void addObject(std::unique_ptr<GameObject> obj, const BoardCell &c);

    // moves an object on the board from its current cell to new position. ignores if object not on board
    // TODO: this is used by moving objects to handle their movements - keep public or allow acces to moving objects only?
    void moveGameObject(GameObject *obj, const BoardCell &new_position);

    // remove the object from the board
    // TODO: used by all objects in death - choose access specifiers
    void removeObject(GameObject *obj);

    // tank API to inform that a shell was shot and the board need to update it's remaining shell count
    // TODO: used by tank objects when shooting - choose access specifiers
    void useTankShell();

    //=== Board State Functions ===
    // Check if there is any object on the cell
    bool isOccupiedCell(const BoardCell &c) const;

    // Check if the specific object exists on board
    bool isObjectOnBoard(const GameObject *obj) const;
};
