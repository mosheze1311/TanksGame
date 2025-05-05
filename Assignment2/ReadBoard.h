#pragma once
#include "ReadBoard.h"
#include "../Logger/Logger.h"
#include "../GameObjects/GameObjects.h"
#include <string>
#include <optional>

class BoardFactory
{
private:
ReadBoard(/* args */) = delete;
    ~ReadBoard() = delete;

    static void logInputError(const string error_message);
    static unique_ptr<GameObject> createGameObjectOfType(GameBoard &board, GameObjectType type);
public:
    static GameBoard* createGameBoard(const string file_path);
};