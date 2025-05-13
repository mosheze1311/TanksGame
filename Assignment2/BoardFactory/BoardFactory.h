#pragma once
#include "../GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObjects.h"

#include <memory>
#include <string>
#include <optional>
#include <set>

using std::make_shared;

//TODO: decide name for class
class BoardFactory
{
private:
    BoardFactory(/* args */) = delete;
    ~BoardFactory() = delete;

    //=== Logger ===
    static void logInputError(const string error_message);

    //=== Parse Input Functions ===
    static bool parseKeyLine(const string &line, size_t line_number, const string &expected_key, size_t &out_value);
    static bool parseBoardLine(GameBoard& board, const string &line, size_t line_number, size_t cols);

    //=== Helper Functions ===
    static unique_ptr<GameObject> createGameObjectOfType(GameBoard &board, GameObjectType type);

    //=== Modify Board Functions ===
    static void setBoardDimensions(GameBoard& board, size_t rows, size_t cols);
    static void setBoardDetails(GameBoard& board,size_t max_steps, size_t shells_per_tank);
    static void addObjectToBoard(GameBoard& borad, const char object_char, int x, int y);

public:
    static bool initGameBoardFromFile(GameBoard& board, const string file_path);
};