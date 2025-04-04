#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <map>
#include "../GameObjects/GameObjects.h"
using namespace std;

class BoardCell
{
public:
    int x;
    int y;

    BoardCell(int x, int y);

    // Overload < operator for using BoardCell as a key in a map
    bool operator<(const BoardCell &other) const;
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

    unordered_map<GameObject*, BoardCell> objects_locations;
    map<BoardCell, GameObject*> board;

public:
    // Constructor
    GameBoard(const string &file_path);

    // Getters
    int getWidth() const;
    int getHeight() const;

    // Board cell checks
    bool isOccupiedCell(const BoardCell &c) const;
    GameObject *objectOnCell(const BoardCell &c);
};