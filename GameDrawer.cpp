#include <iostream>
#include "GameBoard/GameBoard.h"

class GameDrawer
{
private:
    const GameBoard& board;

    string decideCellImage(BoardCell c)
    {
        map<GameObjectType, string> draw_map = {
            {GameObjectType::mine, "💥"},
            {GameObjectType::wall, "⬛"},
            {GameObjectType::tank1, "🚙"},
            {GameObjectType::tank2, "🚜"},
            {GameObjectType::shell, "🟠"}
        };
        vector<string> empty_blocks({"⬜", "⬜"});

        if (this->board.isOccupiedCell(c))
        {
            return draw_map[this->board.objectOnCell(c)->getObjectType()];
        }
        return empty_blocks[(c.x + c.y) % 2];
    }
    
public:
    GameDrawer(const GameBoard& board) : board(board) {};

    void draw()
    {
        string board_drawing=""; // board drawing result 
        static const string game_border_portal = "🌀";
        int border_width = this->board.getWidth() + 2;

        // upper border border
        for (int i = 0; i < border_width; i++)
        {
            board_drawing += game_border_portal;
        }
        board_drawing += "\n";

        // board
        for (int y = 0; y < this->board.getHeight(); y++)
        {
            board_drawing += game_border_portal;
            for (int x = 0; x < this->board.getWidth(); x++)
            {
                BoardCell c(x,y);
                board_drawing += this->decideCellImage(c);
            }
            board_drawing += game_border_portal + "\n";
        }

        for (int i = 0; i < border_width; i++)
        {
            board_drawing += game_border_portal;
        }
        cout << board_drawing << endl;
    };
};
