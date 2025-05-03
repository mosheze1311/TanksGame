#include "../GameObjects/GameObjects.h"
#include "../GameBoard/GameBoard.h"
#include <string>
#include <optional>

class BoardFactory
{
private:
    BoardFactory(/* args */) = delete;
    ~BoardFactory() = delete;

    static void logInputError(const string error_message);
    static GameObject *createGameObjectOfType(GameBoard &board, GameObjectType type);
public:
    static GameBoard* createGameBoard(const string file_path);
};