#include <iostream>
#include "GameBoard.cpp"

class GameDrawer
{
private:
    GameBoard board;

public:
    GameDrawer(GameBoard board) : board(board) {};

    void draw()
    {
        map<game_object, string> draw_map = {
            {game_object::mine, "💥"},
            {game_object::wall, "⬛"},
            {game_object::tank1, "🚙"},
            {game_object::tank2, "🚜"}};
        vector<string> empty_blocks({"⬜", "⬜"});
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
                if (this->board.is_occupied_cell({j, i}))
                {
                    cout << draw_map[this->board.object_on_cell({j, i})];
                }
                else
                {
                    cout << empty_blocks[(i + j) % 2];
                }
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
