#pragma once
#include <unordered_map>
#include <map>
#include "../GameObjects/GameObjects.h"
#include <cstdlib> // for rand()
#include <ctime>   // for time()
#include <unordered_set>

using namespace std;

class BoardCell
{
public:
    int x;
    int y;

    BoardCell();
    BoardCell(int x, int y);
    // Overload < operator for using BoardCell as a key in a map
    bool operator<(const BoardCell &other) const;

    // Overload + to add a Direction
    BoardCell operator+(const Direction dir) const;

    // Overload - to subtract a Direction
    BoardCell operator-(const Direction dir) const;
};

class GameBoard
{
private:
    // Nested class to store board details
    class BoardDetails
    {
    public:
        int height;
        int width;

        int walls;
        int mines;

        int p1_tanks;
        int p2_tanks;
    };

    BoardDetails board_details;

    unordered_map<GameObject *, BoardCell> objects_locations;
    map<BoardCell, unordered_set<GameObject *>> board;

    // create a board cell that fits the board without overflowing
    BoardCell createBoardCell(int x, int y)
    {
        return BoardCell((x + this->board_details.width) % this->board_details.width,
                         (y + this->board_details.height) % this->board_details.height);
    }

    // create a board cell that fits the board without overflowing
    BoardCell createAdjustedBoardCell(BoardCell& c)
    {
        return this->createBoardCell(c.x, c.y);
    }

    void addObjectInternal(GameObject *obj_type, BoardCell c);
    void removeObjectInternal(GameObject *obj);

public :
    // Constructor
    GameBoard(int height, int width);

    // Getters
    int getWidth() const;
    int getHeight() const;

    // Board cell checks
    bool isOccupiedCell(const BoardCell &c) const;
    std::unordered_set<GameObject *> objectOnCell(const BoardCell &c) const;

    // Engage with objects on board
    void addObject(GameObject* obj_type, BoardCell c);
    void moveGameObject(GameObject *obj, BoardCell new_position);
    void removeObject(GameObject *obj, BoardCell position);
    vector<GameObject *> getGameObjects(GameObjectType t) const;
    BoardCell getobjectLocation(GameObject* go) const;

    // temporary function for testing
    void moveTanksRandomly()
    {
        for (pair<GameObject *, BoardCell> iter : objects_locations)
        {
            if (Tank *d = dynamic_cast<Tank *>(iter.first))
            {
                // Randomly choose whether to modify x or y
                bool changeX = std::rand() % 2;

                // Randomly choose to increment or decrement
                int delta = (std::rand() % 2 == 0) ? -1 : 1;

                int newX = iter.second.x;
                int newY = iter.second.y;

                if (changeX)
                {
                    newX += delta;
                }
                else
                {
                    newY += delta;
                }

                BoardCell new_cell = this->createBoardCell(newX, newY);
                moveGameObject(iter.first, new_cell);
            }
        }
    }
};