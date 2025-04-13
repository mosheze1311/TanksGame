#pragma once

#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <vector>
#include <optional>

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

    // Getters
    int getX() const;
    int getY() const;

    // == operator
    bool operator==(const BoardCell &other) const;
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
    BoardCell createBoardCell(int x, int y);

    // create a board cell that fits the board without overflowing
    BoardCell createAdjustedBoardCell(const BoardCell &c);

    // Adds an object to the board - internal use, arguments are treated as valid
    void addObjectInternal(GameObject *obj, BoardCell c);

    // Removes an object from the board - internal use, arguments are treated as valid
    void removeObjectInternal(GameObject *obj);

    void updateObjectCount(GameObject* obj, int incremental = 1);
public:
    // Constructor
    GameBoard(int height, int width);

    // Copy Constructor
    GameBoard(GameBoard &board);

    // get board width
    int getWidth() const;

    // get board height
    int getHeight() const;

    // get count of object type on board
    int getGameObjectCount(GameObjectType type) const;

    // get the total remaining shells count
    int getTotalRemainingShells() const;

    // return all cells with objects on them.
    vector<BoardCell> getOccupiedCells() const;

    // Check if there is any object on the cell
    bool isOccupiedCell(const BoardCell &c) const;

    // Check if the specific object exists on board
    bool isObjectOnBoard(GameObject *obj) const;

    // return a set of all objects on the cell
    std::unordered_set<GameObject *> getObjectsOnCell(const BoardCell &c) const;

    // Add an object to the board on the requested cell
    void addObject(GameObject *obj_type, BoardCell c);

    // moves an object on the board from its current cell to new position. ignores if object not on board
    void moveGameObject(GameObject *obj, BoardCell new_position);

    // remove the object from the board
    void removeObject(GameObject *obj);

    // get all objects of a certain GameObjectType that exist on board
    vector<GameObject *> getGameObjects(GameObjectType t) const;

    // get all objects on the board
    vector<GameObject *> getAllGameObjects();

    // get an optional cell location of an object on board. if object not on board, return nullopt.
    std::optional<BoardCell> getObjectLocation(GameObject *go) const;

    // tank API to inform that a shell was shot and the board need to update it's remaining shell count
    void useTankShell();

    // TODO: this is a temporary function for testing - delete later
    void moveTanksRandomly();
};
