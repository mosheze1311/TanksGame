#include "GameBoard.h"

// Constructor Implementation
BoardCell::BoardCell(int x, int y) : x(x), y(y) {}

// Operator < overload for using BoardCell as a map key
bool BoardCell::operator<(const BoardCell &other) const
{
    return (x < other.x) || (x == other.x && y < other.y);
}

// Constructor: Reads game board details from a file
GameBoard::GameBoard(const string &file_path)
{
    ifstream file(file_path);
    if (!file)
    {
        cerr << "Error: Could not open file " << file_path << endl;
        return; // Handle file open failure
    }

    file >> board_details.height >> board_details.width >> board_details.p1_tanks >> board_details.p2_tanks >> board_details.walls >> board_details.mines;

    // TODO: Read all game objects from the file
    int n = board_details.p1_tanks;
    for (int i = 0; i < n; i++)
    {
        // Implementation for reading tank positions
    }

    file.close();
}

// Getters
int GameBoard::getWidth() const
{
    return board_details.width;
}

int GameBoard::getHeight() const
{
    return board_details.height;
}

// Check if a cell is occupied
bool GameBoard::isOccupiedCell(const BoardCell &c) const
{
    return board.find(c) != board.end();
}

// Get the object at a cell (returns pointer or NULL)
GameObject *GameBoard::objectOnCell(const BoardCell &c)
{
    if (isOccupiedCell(c))
    {
        return &(board[c]);
    }
    return nullptr;
}