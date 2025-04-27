#include "GameCollisionHandler.h"
#include <iostream>

//=== Constructors ===
GameCollisionHandler::GameCollisionHandler(GameBoard &board) : previous_board(board) {}

GameCollisionHandler::~GameCollisionHandler() {}

//=== Static Members - Collision maps ===
const CollisionMap GameCollisionHandler::explosion_map = {
    {GameObjectType::MINE, {{GameObjectType::TANK1, 1}, {GameObjectType::TANK2, 1}}},
    {GameObjectType::TANK1, {{GameObjectType::MINE, 1}, {GameObjectType::TANK1, 2}, {GameObjectType::TANK2, 1}, {GameObjectType::SHELL, 1}}},
    {GameObjectType::TANK2, {{GameObjectType::MINE, 1}, {GameObjectType::TANK1, 1}, {GameObjectType::TANK2, 2}, {GameObjectType::SHELL, 1}}},
    {GameObjectType::SHELL, {{GameObjectType::SHELL, 2}, {GameObjectType::TANK1, 1}, {GameObjectType::TANK2, 1}, {GameObjectType::WALL, 1}}},
    {GameObjectType::WALL, {{GameObjectType::SHELL, 1}}}};

const CollisionMap GameCollisionHandler::prevention_map = {
    {GameObjectType::MINE, {{GameObjectType::WALL, 1}}},
    {GameObjectType::TANK1, {{GameObjectType::WALL, 1}}},
    {GameObjectType::TANK2, {{GameObjectType::WALL, 1}}},
    {GameObjectType::SHELL, {}},
    {GameObjectType::WALL, {{GameObjectType::TANK1, 1}, {GameObjectType::TANK2, 1}, {GameObjectType::MINE, 1}}}};

//=== Member Functions ===
void GameCollisionHandler::handleCollisions(GameBoard &updated_board)
{
    this->positionNewShellsOnPreviousBoard(updated_board); // mid step collision preparing

    this->handleMidStepCollisions(updated_board);
    this->handleEndOfStepCollisions(updated_board);

    this->previous_board = updated_board;
};

void GameCollisionHandler::handleMidStepCollisions(GameBoard &updated_board) const
{
    // a mid step collision occurs when two objects switch lcations between steps.
    for (GameObject *obj : updated_board.getAllGameObjects())
    {

        optional<BoardCell> opt_current_loc = updated_board.getObjectLocation(obj);
        optional<BoardCell> opt_previous_loc = previous_board.getObjectLocation(obj);
        if (!opt_current_loc || !opt_previous_loc) // object is no longer on board
            continue;
        BoardCell current_location = *opt_current_loc;
        BoardCell previous_location = *opt_previous_loc;
       
        map<GameObjectType, int> should_expload = this->getCollisionCounterMap(GameCollisionHandler::explosion_map, obj->getObjectType());
        unordered_set<GameObject *> objects_that_were_on_my_current_cell = this->previous_board.getObjectsOnCell(current_location); // petntial mid step collisions
        for (GameObject *other : objects_that_were_on_my_current_cell)
        {
            if(!updated_board.isObjectOnBoard(other))
                continue;
                
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
                // TODO: this is ok for now since all moving objects have 1 hp. to ensure only 1 hit per collision later - list mid step colliders and handle at the end
                obj->gotHit(1);
                other->gotHit(1);
            }
        }
    }
}

void GameCollisionHandler::handleEndOfStepCollisions(GameBoard &updated_board) const
{
    // an end of step collision occurs when two objects are on the same location at the end of the step.
    for (BoardCell &cell : updated_board.getOccupiedCells())
    {
        map<GameObjectType, int> exploding_objects;
        unordered_set<GameObject *> cell_objects = updated_board.getObjectsOnCell(cell);

        for (GameObject *obj : cell_objects)
        {
            map<GameObjectType, int> should_expload = this->getCollisionCounterMap(GameCollisionHandler::explosion_map, obj->getObjectType());
            for (auto iter: should_expload){
                int value = iter.second;
                if(exploding_objects.find(iter.first) != exploding_objects.end())
                    value = 1;
                exploding_objects[iter.first] = value;
            }
            exploding_objects.insert(should_expload.begin(), should_expload.end());
        }

        for (GameObject *obj : cell_objects)
        {
            if (exploding_objects.find(obj->getObjectType()) != exploding_objects.end() && exploding_objects[obj->getObjectType()] == 1)
            {
                obj->gotHit(1);
            }
        }
    }
}

void GameCollisionHandler::positionNewShellsOnPreviousBoard(const GameBoard& updated_board){
    vector<GameObject*> all_shells = updated_board.getGameObjects(GameObjectType::SHELL);
    for(GameObject* shell_obj : all_shells){
        if(!(this->previous_board.isObjectOnBoard(shell_obj))){
            Shell* shell = static_cast<Shell*>(shell_obj);
            BoardCell shell_current_location = updated_board.getObjectLocation(shell_obj).value(); // immediate unwrap optioanl since object is on board
            BoardCell shell_shot_location = updated_board.getNextCellInDirection(shell_current_location, DirectionUtils::getOppositeDirection(shell->getDirection()));
            this->previous_board.addObject(shell, shell_shot_location);
        }
    }

}

    const CollisionCountersMap
    GameCollisionHandler::getCollisionCounterMap(CollisionMap collision_map, GameObjectType objType)
{
    auto iter = collision_map.find(objType);
    if (iter != collision_map.end())
    {
        return iter->second;
    }
    return {};
}

//=== Static Functions ===
bool GameCollisionHandler::isObjectAllowedToStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    return !GameCollisionHandler::isCollision(GameCollisionHandler::prevention_map, board, obj_type, c);
}

bool GameCollisionHandler::canObjectSafelyStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    if (!GameCollisionHandler::isObjectAllowedToStepOn(board, obj_type, c))
    {
        return false;
    }

    return !GameCollisionHandler::isCollision(GameCollisionHandler::explosion_map, board, obj_type, c);
}

bool GameCollisionHandler::isCollision(const CollisionMap collision_map, const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    auto objects_on_cell = board.getObjectsOnCell(c);
    map<GameObjectType, int> counters;
    for (auto obj : objects_on_cell)
    {
        counters[obj->getObjectType()] += 1;
    }

    for (auto count_pair : counters)
    {
        const CollisionCountersMap collision_counter_map = getCollisionCounterMap(collision_map, obj_type);
        if (collision_counter_map.empty())
            continue;

        auto iter = collision_counter_map.find(count_pair.first);
        if (iter == collision_counter_map.end()) 
            continue;

        if (iter->second <= count_pair.second)
            return true;

    }
    return false;
}