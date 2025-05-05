#pragma  once
#include "common/PlayerFactory.h"
#include "common/TankAlgorithmFactory.h"
#include "GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include "GameDrawer/GameDrawer.h"

#include <fstream>
#include <sstream>
#include <string>
#include <set>
/*
 TODO: I added the 'createGameObjectOfType' method (that was originally on the Collision Handler)
        It is using 'new' and should not remain - only here to allow testing for the 'readBoard' method.
*/ 

class GameManager{
private:
    //=== Attributes ===
    GameBoard board;
    size_t max_steps;
    size_t tanks_num_shells;

    const PlayerFactory& player_factory;
    const TankAlgorithmFactory& tank_algorithm_factory;
    

    //=== Handle Input File Functions ===
    static void logInputError(const string error_description);
    bool parseKeyLine(const string &line, size_t line_number, const string &expected_key, size_t &out_value) const;
    bool parseBoardLine(const string& line, size_t line_number ,size_t cols);
    bool isValidObjectChar(char obj_char) const;
    GameObject* createGameObjectOfType(GameObjectType type);

    //=== Manipulate Board Functions ===
    void setBoardDimensions(size_t rows, size_t cols);
    void addObjectToBoard(const char object_char, size_t x, size_t y);

public:
    //~GameManager();
    GameManager(const PlayerFactory& player_factory, const TankAlgorithmFactory& tank_algorithm_factory);
    bool readBoard(std::string input_file_path);
    void run();
};

GameManager::GameManager(const PlayerFactory &player_factory, const TankAlgorithmFactory &tank_algorithm_factory)
    : board(0, 0), player_factory(player_factory), tank_algorithm_factory(tank_algorithm_factory)
{
}

void GameManager::run(){
    //TODO: add this function
}
//=== Input File Handling ===
bool GameManager::readBoard(std::string input_file_path)
{
    ifstream file(input_file_path);

    // Handle file open failure
    if (!file)
    {
        GameManager::logInputError("Can't open the input file");
        return false;
    }

    string line;
    // read map name (name is currently ignored)
    size_t line_number = 1;
    if (!(std::getline(file, line)))
    {
        GameManager::logInputError("Line 1: file does not contain the map name");
        return false;
    };

    // read lines 2-5 with the key-value pairs
    size_t rows, cols;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "MaxSteps", this->max_steps))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "NumShells", this->tanks_num_shells))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Rows", rows))
        return false;
    if (!std::getline(file, line) || !parseKeyLine(line, ++line_number, "Cols", cols))
        return false;

    this->setBoardDimensions(rows, cols);
    // read the board itself
    size_t board_row_number = 0;

    while (board_row_number < rows && std::getline(file, line))
    {
        line_number++;      // advance the read line if read any;
        board_row_number++; // advance board row

        if (!this->parseBoardLine(line, board_row_number, cols))
        {
            return false; // return if some error in board - should not happen
        }
    }

    GameDrawer drawer(this->board,DrawingType::REGULAR);
    drawer.draw();
    return true;
}

bool GameManager::parseKeyLine(const string &line, size_t line_number, const string &expected_key, size_t &out_value) const
{
    istringstream iss(line);
    string key, equal_sign;
    size_t value;

    if (!(iss >> key))
    {
        GameManager::logInputError("Line " + std::to_string(line_number) + ": Missing key.\n");
        return false;
    }
    if (key != expected_key)
    {
        GameManager::logInputError( "Line " + std::to_string(line_number) + ": Unexpected key '" + key + "', expected '" + expected_key + "'.\n");
        return false;
    }
    if (!(iss >> equal_sign) || equal_sign != "=")
    {
        GameManager::logInputError("Line " + std::to_string(line_number) + ": Expected '=' after key.");
        return false;
    }

    if (!(iss >> value))
    {
        GameManager::logInputError("Line " + std::to_string(line_number) + ": Expected numeric value after '='.");
        return false;
    }

    out_value = value;
    return true;
}

bool GameManager::parseBoardLine(const string &line, size_t line_number ,size_t cols)
{
    cols = min(cols, line.length()); // ensure no overflowing lines.
    
    char obj_char;
    for(size_t col = 0; col < cols; col++){
        obj_char = line[col];
        if (!this->isValidObjectChar(obj_char))
            continue;

        this->addObjectToBoard(obj_char, col, line_number);
    }

    return true;
}

bool GameManager::isValidObjectChar(char obj_char) const{
    // TODO: maybe move to GameObjectType enum class as helper function
    set<char> valid_chars({'1','2','#', '@'});

    return (valid_chars.find(obj_char)!=valid_chars.end());
}

void GameManager::logInputError(const string error_description)
{
    // this is a static function
    Logger::input().logError(error_description);
}

//=== Board Manipulations ===
void GameManager::setBoardDimensions(size_t rows, size_t cols){
    this->board.setWidth(cols);
    this->board.setHeight(rows);
}

void GameManager::addObjectToBoard(const char object_char, size_t x, size_t y)
{
    // TODO: fix this and match to GameBoard - currently not correct and gives char instead of GameObject*

    this->board.addObject(createGameObjectOfType((GameObjectType)object_char), {static_cast<int>(x), static_cast<int>(y)});
}

GameObject* GameManager::createGameObjectOfType(GameObjectType type)
{
    switch (type)
    {
    case GameObjectType::TANK1:
        return new Tank(this->board, type, Direction::LEFT);
    case GameObjectType::TANK2:
        return new Tank(this->board, type, Direction::RIGHT);
    case GameObjectType::WALL:
        return new Wall(this->board);
    case GameObjectType::MINE:
        return new Mine(this->board);
    default:
        return ; // Handle unknown type safely
    }
}