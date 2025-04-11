#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include "../Logger/Logger.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// TODO: log errors into input_errors.txt
class BoardFactory
{
private:
    GameObject *createGameObjectOfType(GameBoard &board, GameObjectType type);
    void logInputError(const string error_message) const;

public:
    BoardFactory(/* args */);
    ~BoardFactory();

    GameBoard createGameBoard(const string file_path);
};