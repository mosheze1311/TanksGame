#include "GameCollisionHandler.h"

// TODO: change collision handling to fit multiple tanks.
//=== Constructors ===
GameCollisionHandler::GameCollisionHandler(GameBoard &board) : previous_board(board) {}

GameCollisionHandler::~GameCollisionHandler() {}

//=== Static Members - Collision maps ===
// TODO: rethink about the counters of each CollisionType.
const CollisionMap GameCollisionHandler::explosion_map = {
    {CollisionObjectType::MINE, {CollisionObjectType::TANK}},
    {CollisionObjectType::TANK, {CollisionObjectType::MINE, CollisionObjectType::TANK, CollisionObjectType::SHELL}},
    {CollisionObjectType::SHELL, {CollisionObjectType::SHELL, CollisionObjectType::TANK, CollisionObjectType::WALL}},
    {CollisionObjectType::WALL, {CollisionObjectType::SHELL}}};

const CollisionMap GameCollisionHandler::prevention_map = {
    {CollisionObjectType::MINE, {CollisionObjectType::WALL}},
    {CollisionObjectType::TANK, {CollisionObjectType::WALL}},
    {CollisionObjectType::SHELL, {}},
    {CollisionObjectType::WALL, {CollisionObjectType::TANK, CollisionObjectType::MINE}}};

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
        std::optional<BoardCell> opt_current_loc = updated_board.getObjectLocation(obj);
        std::optional<BoardCell> opt_previous_loc = previous_board.getObjectLocation(obj);
        if (!opt_current_loc || !opt_previous_loc) // object is no longer on board. TODO: think of new shells and how it effects them
            continue;
        
        BoardCell current_location = *opt_current_loc;
        BoardCell previous_location = *opt_previous_loc;

        std::unordered_set<CollisionObjectType> colliders = this->getCollidingTypes(GameCollisionHandler::explosion_map, obj->getObjectType());
        std::unordered_set<GameObject *> objects_that_were_on_my_current_cell = this->previous_board.getObjectsOnCell(current_location); // potential mid step collisions
        for (GameObject *other : objects_that_were_on_my_current_cell)
        {
            if (!updated_board.isObjectOnBoard(other))
                continue;

            if (obj == other)
                continue;

            if (colliders.find(CollisionObjectTypeUtils::fromGameObjectType(obj->getObjectType())) == colliders.end())
                continue;

            std::optional<BoardCell> opt_other_current_loc = updated_board.getObjectLocation(other);
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
    // each collision cause 1 hp hit
    for (BoardCell &cell : updated_board.getOccupiedCells())
    {
        std::unordered_set<GameObject *> cell_objects = updated_board.getObjectsOnCell(cell);
        for (auto iter = cell_objects.begin(); iter != cell_objects.end();)
        {
            GameObject *obj1 = *iter;
            auto iter2 = ++iter;
            if (!updated_board.isObjectOnBoard(obj1))
                continue;
            std::unordered_set<CollisionObjectType> collide_with = getCollidingTypes(explosion_map, obj1->getObjectType());
            for (;iter2 != cell_objects.end(); ++iter2)
            {
                GameObject *obj2 = *iter2;
                if (!updated_board.isObjectOnBoard(obj2))
                    continue;
                CollisionObjectType obj2_type = CollisionObjectTypeUtils::fromGameObjectType(obj2->getObjectType());
                if (collide_with.find(obj2_type) != collide_with.end())
                { 
                    // should explode each other
                    if (updated_board.isObjectOnBoard(obj1))
                        obj1->gotHit(1);
                        
                    obj2->gotHit(1);
                }
            }
        }
    }
}

void GameCollisionHandler::positionNewShellsOnPreviousBoard(const GameBoard &updated_board)
{
    std::vector<GameObject *> all_shells = updated_board.getGameObjects(GameObjectType::SHELL);
    for (GameObject *shell_obj : all_shells)
    {
        if (!(this->previous_board.isObjectOnBoard(shell_obj)))
        {
            Shell *shell = static_cast<Shell *>(shell_obj);
            BoardCell shell_current_location = updated_board.getObjectLocation(shell_obj).value(); // immediate unwrap optioanl since object is on board
            BoardCell shell_shot_location = GameBoardUtils::getNextCellInDirection(shell_current_location, DirectionUtils::getOppositeDirection(shell->getDirection()), updated_board.getWidth(), updated_board.getHeight());
            this->previous_board.addObject(shell, shell_shot_location);
        }
    }
}

const std::unordered_set<CollisionObjectType> GameCollisionHandler::getCollidingTypes(CollisionMap collision_map, GameObjectType objType)
{
    auto iter = collision_map.find(CollisionObjectTypeUtils::fromGameObjectType(objType));
    if (iter != collision_map.end())
    {
        return iter->second;
    }
    return {};
}

//=== Static Functions ===
bool GameCollisionHandler::isObjectAllowedToStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::prevention_map, board, obj_type, c);
}

bool GameCollisionHandler::canObjectSafelyStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    if (!GameCollisionHandler::isObjectAllowedToStepOn(board, obj_type, c))
    {
        return false;
    }

    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::explosion_map, board, obj_type, c);
}

bool GameCollisionHandler::isCollidingOnCell(const CollisionMap collision_map, const GameBoard &board, GameObjectType obj_type, BoardCell c)
{
    auto objects_on_cell = board.getObjectsOnCell(c);
    const std::unordered_set<CollisionObjectType> collidors = getCollidingTypes(collision_map, obj_type);

    if (collidors.empty())
        return false;

    for (GameObject* object : objects_on_cell)
    {
        CollisionObjectType t = CollisionObjectTypeUtils::fromGameObjectType(object->getObjectType());
        if (collidors.find(t) != collidors.end())
            return true;
    }
    
    return false;
}

// Overloaded functions for SatelliteView
bool GameCollisionHandler::isObjectAllowedToStepOn(const SatelliteAnalyitcsView& sat_view, GameObjectType obj_type, BoardCell c)
{
    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::prevention_map, sat_view, obj_type, c);
}

bool GameCollisionHandler::canObjectSafelyStepOn(const SatelliteAnalyitcsView& sat_view, GameObjectType obj_type, BoardCell c)
{
    if (!GameCollisionHandler::isObjectAllowedToStepOn(sat_view, obj_type, c))
    {
        return false;
    }

    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::explosion_map, sat_view, obj_type, c);
}

bool GameCollisionHandler::isCollidingOnCell(const CollisionMap collision_map, const SatelliteAnalyitcsView &sat_view, GameObjectType obj_type, BoardCell c)
{
    char cell_obj_char = sat_view.getObjectAt(c.getX(), c.getY()).first;
    
    // handle empty space and out of bounds case
    if (!GameObjectTypeUtils::isValidObjectChar(cell_obj_char))
        return false;

    GameObjectType cell_obj_type = static_cast<GameObjectType>(cell_obj_char);
    CollisionObjectType cell_obj_coll_type = CollisionObjectTypeUtils::fromGameObjectType(cell_obj_type);
    const std::unordered_set<CollisionObjectType> collidors = getCollidingTypes(collision_map, obj_type);

    return collidors.find(cell_obj_coll_type) != collidors.end();
}