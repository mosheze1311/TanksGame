#include "GameCollisionHandler.h"
// TODO: 
// instead of removing items from board on collision, 
// items should be 'hit' and handle their own board situation with according to their hp

// Define the explosion_map with its initial values
std::map<GameObjectType, std::unordered_set<GameObjectType>> GameCollisionHandler::explosion_map = {
    {GameObjectType::mine, {GameObjectType::tank1, GameObjectType::tank2}},
    {GameObjectType::tank1, {GameObjectType::mine, GameObjectType::tank1, GameObjectType::tank2, GameObjectType::shell}},
    {GameObjectType::tank2, {GameObjectType::mine, GameObjectType::tank1, GameObjectType::tank2, GameObjectType::shell}},
    {GameObjectType::shell, {GameObjectType::shell, GameObjectType::tank1, GameObjectType::tank2, GameObjectType::wall}},
    {GameObjectType::wall, {GameObjectType::shell}}};

GameCollisionHandler::GameCollisionHandler(GameBoard &board) : previous_board(board) {}

GameCollisionHandler::~GameCollisionHandler() {}

void GameCollisionHandler::handleCollisions(GameBoard &updated_board)
{
    this->handleMidStepCollisions(updated_board);
    this->handleEndOfStepCollisions(updated_board);

    this->previous_board = updated_board;
};

void GameCollisionHandler::handleMidStepCollisions(GameBoard &updated_board)
{
    // a mid step collision occurs when two objects switch lcations between steps.
    for (GameObject *obj : updated_board.getAllGameObjects())
    {
        unordered_set<GameObjectType> should_expload = this->getExplosionList(obj->getObjectType());

        optional<BoardCell> opt_current_loc = updated_board.getObjectLocation(obj);
        optional<BoardCell> opt_previous_loc = previous_board.getObjectLocation(obj);
        if (!opt_current_loc || !opt_previous_loc) // should not happen
            continue;

        BoardCell current_location = *opt_current_loc;
        BoardCell previous_location = *opt_previous_loc;
        unordered_set<GameObject *> objects_that_were_on_my_current_cell = this->previous_board.getObjectsOnCell(current_location);
        for (GameObject *other : objects_that_were_on_my_current_cell)
        {
            if (obj == other)
                continue;
                
            if (should_expload.find(other->getObjectType()) == should_expload.end())
                continue;

            optional<BoardCell> opt_other_current_loc = updated_board.getObjectLocation(other);
            if (!opt_other_current_loc)
                continue;

            BoardCell other_current_loc = *opt_other_current_loc;
            if (other_current_loc == previous_location)
            {
                updated_board.removeObject(obj);
                updated_board.removeObject(other);
            }
        }
    }
}

void GameCollisionHandler::handleEndOfStepCollisions(GameBoard &updated_board)
{
    // an end of step collision occurs when two objects are on the same location at the end of the step.
    for (BoardCell &cell : updated_board.getOccupiedCells())
    {
        unordered_set<GameObjectType> exploding_objects;
        unordered_set<GameObject *> cell_objects = updated_board.getObjectsOnCell(cell);

        for (GameObject *obj : cell_objects)
        {
            unordered_set<GameObjectType> should_expload = this->getExplosionList(obj->getObjectType());
            exploding_objects.insert(should_expload.begin(), should_expload.end());
        }

        for (GameObject *obj : cell_objects)
        {
            if (exploding_objects.find(obj->getObjectType()) != exploding_objects.end())
            {
                updated_board.removeObject(obj);
            }
        }
    }
}

std::unordered_set<GameObjectType> GameCollisionHandler::getExplosionList(GameObjectType objType)
{

    if (GameCollisionHandler::explosion_map.find(objType) != GameCollisionHandler::explosion_map.end())
    {
        return GameCollisionHandler::explosion_map[objType];
    }

    return {};
}