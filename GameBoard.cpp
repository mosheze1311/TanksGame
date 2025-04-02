#include <iostream>
#include <fstream>
#include <vector>
#include <map>

// #include "BoardDetails.cpp"
#include "GameObjects.h"

#define CELL pair<int, int>

using namespace std;

class GameBoard
{
private:
    class BoardDetails{
    public:
        int height;
        int width;

        int walls;
        int mines;

        int p1_tanks;
        int p2_tanks;
    };
    
    BoardDetails board_details;
    map<game_object, vector<CELL>> objects_locations;
    map<int, map<int, game_object>> board;


public:
    GameBoard(string file_path)
    {
        ifstream file(file_path);
        if (!file)
        {
            // todo: handle can't open file
        }

        // todo: handle case where input file is not good
        file >>
            this->board_details.height >>
            this->board_details.width >>
            this->board_details.p1_tanks >>
            this->board_details.p2_tanks >>
            this->board_details.walls >>
            this->board_details.mines;
        
        // todo: complete code for reading all game objects
        int n = this->board_details.p1_tanks;
        for (int i = 0; i < n; i++){

        }

        file.close(); // Close file
    }

    int getWidth(){
        return this->board_details.width;
    }
    int getHeight()
    {
        return this->board_details.height;
    }

    bool is_occupied_cell(CELL c)
    {
        if (board.find(c.first) == board.end())
        {
            return false;
        }
        if (board[c.first].find(c.second) == board[c.first].end())
        {
            return false;
        }
        return true;
    }

    game_object object_on_cell(CELL c){
        return this->board[c.first][c.second];
    }
};