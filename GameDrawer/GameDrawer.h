#pragma once
#include <iostream>
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "DrawingTypes.h"
#include <optional>

class GameDrawer
{
private:
    // ===Attributes===
    const GameBoard &board;
    const DrawingType dt;

    // ===Functions===
    string decideCellImage(BoardCell c)
    {
        vector<string> empty_blocks({"⬜", "⬜"});

        if (this->board.isOccupiedCell(c))
        {
            unordered_set<GameObject *> objects = this->board.getObjectsOnCell(c);
            GameObject *first = *(objects.begin());
            return first->getDrawing(this->dt);
        }
        return empty_blocks[(c.getX() + c.getY()) % 2];
    }

public:
    // ===Constructor===
    GameDrawer(const GameBoard &board, DrawingType dt=DrawingType::NONE) : board(board), dt(dt) {};
    
    // ===Functions===
    void draw()
    {
        string board_drawing = ""; // board drawing result
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
                BoardCell c(x, y);
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
