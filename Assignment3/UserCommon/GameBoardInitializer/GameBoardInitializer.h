#pragma once

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../Logger/Logger.h"
#include "../Utils/GameObjectTypeUtils.h"


#include <functional>
#include <memory>
#include <string>

class GameBoardInitializer
{
    /*
    GameBoardInitializer is a static utility class responsible for handling input files.
    It encapsulates all logic related to reading and parsing the input file used to
    initialize the GameBoard.

    The GameBoard class is aware of this class and delegates the responsibility of
    input file processing to GameBoardInitializer, maintaining separation of concerns.
    */
private:
    static const char empty_space_char = ' ';

    GameBoardInitializer() = delete;
    ~GameBoardInitializer() = delete;

    // === Logger === //
    static void logInputError(const std::string &error_message, bool can_recover);

    // === Parse Input Functions === //
    static bool parseKeyLine(const std::string &line, size_t line_number, const std::string &expected_key, size_t &out_value);
    static bool parseBoardLine(const std::string &line, size_t expected_width, size_t row, GameBoard &board,
                               std::function<void(std::unique_ptr<GameObject>, const BoardCell &)> addObjectToBoard);

    // === Helper Functions === //
    static std::unique_ptr<GameObject> createGameObjectOfType(GameBoard &board, GameObjectType type);

public:
    static bool initGameBoardFromFile(const std::string &input_file_path,
                                      GameBoard &board,
                                      std::function<void(int, int, int, int)> setBoardDetails,
                                      std::function<void(std::unique_ptr<GameObject>, const BoardCell &)> addObjectToBoard);
};