#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <vector>
#include <optional>
#include "../GameObjects/GameObjects.h"
using namespace std;

// Forward declarations
class GameObject;
enum class GameObjectType;
enum class Direction;

// Classes declarations
class BoardCell
{
private:
    int x;
    int y;

public:
    // Empty constructor - do not delete
    BoardCell();

    // Constructor
    BoardCell(int x, int y);

    // Overload < operator for using BoardCell as a key in a map
    bool operator<(const BoardCell &other) const;

    // Overload + to add a Direction
    BoardCell operator+(const Direction dir) const;

    // Overload - to subtract a Direction
    BoardCell operator-(const Direction dir) const;

    // == operator
    bool operator==(const BoardCell &other) const;

    // Getters
    int getX() const;
    int getY() const;
};

class GameBoard
{
private:
    // === Nested class to store board details ===
    class BoardDetails
    {
    public:
        // === Attributes ===
        int height;
        int width;

        int walls;
        int mines;

        int p1_tanks;
        int p2_tanks;

        int shells;
        int remaining_shells;
        // === Constructor ===
        BoardDetails(int height, int width) : height(height), width(width), walls(0), mines(0), p1_tanks(0), p2_tanks(0), shells(0), remaining_shells(0) {};
    };

    // === Attributes ===
    BoardDetails board_details;
    unordered_map<GameObject *, BoardCell> objects_locations;
    map<BoardCell, unordered_set<GameObject *>> board;

    // === Functions ===
    // create a board cell that fits the board without overflowing
    BoardCell createBoardCell(int x, int y) const;

    // create a board cell that fits the board without overflowing
    BoardCell createAdjustedBoardCell(const BoardCell &c) const;

    // Adds an object to the board - internal use, arguments are treated as valid
    void addObjectInternal(GameObject *obj, const BoardCell& c);

    // Removes an object from the board - internal use, arguments are treated as valid
    void removeObjectInternal(GameObject *obj);

    // Updates the count of a specific object type on the board by adding incremental
    void updateObjectCount(GameObject* obj, int incremental);


    
public:
    // Constructor
    GameBoard(int height, int width);

    // Copy Constructor
    GameBoard(GameBoard &board);

    // Assignment Operator
    GameBoard &operator=(const GameBoard &other);
    
    //=== Getters ===
    // get board width
    int getWidth() const;
    
    // get board height
    int getHeight() const;

    // get count of object type on board
    int getGameObjectCount(GameObjectType type) const;

    // get the total remaining shells count
    int getTotalRemainingShells() const;

    // get all cells with objects on them.
    vector<BoardCell> getOccupiedCells() const;

    // get a set of all objects on the cell
    std::unordered_set<GameObject *> getObjectsOnCell(const BoardCell &c) const;

    // get all objects of a certain GameObjectType that exist on board
    vector<GameObject *> getGameObjects(GameObjectType t) const;

    // get all objects on the board
    vector<GameObject *> getAllGameObjects();

    // get all tanks for a player
    vector<Tank *> getTanks(GameObjectType t) const;

    // get an optional cell location of an object on board. if object not on board, return nullopt.
    std::optional<BoardCell> getObjectLocation(const GameObject *go) const;

    // get neigbhor cells
    std::vector<BoardCell> getAdjacentCells(const BoardCell &curr_cell) const;

    BoardCell getNextCellInDirection(const BoardCell &c, const Direction dir) const;

    //=== Modify Board Functions ===
    // Add an object to the board on the requested cell
    void addObject(GameObject *obj_type, const BoardCell& c);

    // moves an object on the board from its current cell to new position. ignores if object not on board
    void moveGameObject(GameObject *obj, const BoardCell& new_position);

    // remove the object from the board
    void removeObject(GameObject *obj);

    // tank API to inform that a shell was shot and the board need to update it's remaining shell count
    void useTankShell();

    //=== Board State Functions ===
    // Check if there is any object on the cell
    bool isOccupiedCell(const BoardCell &c) const;

    // Check if the specific object exists on board
    bool isObjectOnBoard(GameObject *obj) const;

    // get distance of 2 cells
    int distance(const BoardCell& first, const BoardCell& second) const;

    bool isStraightLine(BoardCell from, BoardCell to) const; // maybe add modulo

    bool isDirectionMatch(BoardCell from, BoardCell to, Direction dir) const;
};
