#pragma once

#include "DrawingTypes.h"

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"

#include <iostream>
#include <optional>
#include <thread>
#include <chrono>

#define FPS 60
using namespace UserCommon_211388913_322330820;

class GameDrawer
{
private:
    // === Attributes === //
    const GameBoard &board;
    const DrawingType appearence;

    // === Functions === //
    std::string decideCellImage(const BoardCell &c) const
    {
        std::vector<std::string> empty_blocks({"⬜", "⬜"});
        // std::vector<std::string> empty_blocks({"🟧", "⬜"}); // for chess pattern

        if (this->board.isOccupiedCell(c))
        {
            std::unordered_set<GameObject *> objects = this->board.getObjectsOnCell(c);
            GameObject *selected = *(objects.begin());

            for (GameObject* obj : objects){
                if(obj->getObjectType() == GameObjectType::SHELL){
                    selected = obj;
                    break;
                }
            }
            return selected->getDrawing(this->appearence);
        }
        return empty_blocks[(c.getX() + c.getY()) % 2];
    }

    void clearConsole() const
    {
        std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
    }

public:
    // === Constructor === //
    GameDrawer(const GameBoard &board, DrawingType dt = DrawingType::NONE) : board(board), appearence(dt) {};

    // === Functions === //
    void draw() const
    {
        if (this->appearence == DrawingType::NONE)
            return;

        clearConsole();
        std::string board_drawing = ""; // board drawing result
        static const std::string game_border_portal = "🌀";
        int border_width = this->board.getWidth() + 2;

        // upper border border
        for (int i = 0; i < border_width; i++)
        {
            board_drawing += game_border_portal;
        }
        board_drawing += "\n";

        // board
        for (size_t y = 0; y < this->board.getHeight(); y++)
        {
            board_drawing += game_border_portal;
            for (size_t x = 0; x < this->board.getWidth(); x++)
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
        std::cout << board_drawing << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / FPS));
    };
};
