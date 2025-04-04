#include <iostream>
#include "GameBoard/GameBoard.h"

class GameDrawer
{
private:
    GameBoard board;

    char decideCellImage(BoardCell c)
    {
        map<game_object, string> draw_map = {
            {game_object::mine, "💥"},
            {game_object::wall, "⬛"},
            {game_object::tank1, "🚙"},
            {game_object::tank2, "🚜"}};
        vector<string> empty_blocks({"⬜", "⬜"});

        if (this->board.isOccupiedCell(c))
        {
            cout << draw_map[this->board.objectOnCell(c)->getType()];
        }
        else
        {
            cout << empty_blocks[(c.x + c.y) % 2];
        }
    }
    
public:
    GameDrawer(GameBoard board) : board(board) {};

    void draw()
    {
        string game_border_portal = "🌀";
        
        for (int i = 0; i < this->board.getWidth() + 2; i++)
        {
            cout << game_border_portal;
        }
        cout << endl;

        for (int i = 0; i < this->board.getHeight(); i++)
        {
            cout << game_border_portal;
            for (int j = 0; j < this->board.getWidth(); j++)
            {
                BoardCell c(j,i);
                cout << this->decideCellImage(c);
            }
            cout << game_border_portal << endl;
        }

        for (int i = 0; i < this->board.getWidth() + 2; i++)
        {
            cout << game_border_portal;
        }
        cout << endl;
    };
};
