#include "GameCollisionHandler.h"

//=== Constructors ===
GameCollisionHandler::GameCollisionHandler(GameBoard &board) : previous_board(board) {}

GameCollisionHandler::~GameCollisionHandler() {}

// === Static Members - Collision maps === //
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

// === Member Functions === //
void GameCollisionHandler::handleCollisions(GameBoard &updated_board)
{
    this->positionNewShellsOnPreviousBoard(updated_board); // mid step collision preparing

    this->handleMidStepCollisions(updated_board);
    this->handleEndOfStepCollisions(updated_board);

    this->previous_board = GameBoardShallowCopy(updated_board);
};

void GameCollisionHandler::handleMidStepCollisions(GameBoard &updated_board) const
{
    // a mid step collision occurs when two objects switch lcations between steps by crossing each other (opposite directions).
    for (GameObject *obj : updated_board.getAllGameObjects())
    {
        std::optional<BoardCell> opt_current_loc = updated_board.getObjectLocation(obj);
        std::optional<BoardCell> opt_previous_loc = previous_board.getObjectLocation(obj);
        if (!opt_current_loc || !opt_previous_loc) // object is no longer on board. (previous fail should not happen since adding new shells to previous board, and no other new objects should be born)
            continue;

        BoardCell current_location = *opt_current_loc;
        BoardCell previous_location = *opt_previous_loc;

        if (current_location == previous_location) // object did not move so cant have mid step collision
            continue;

        std::unordered_set<CollisionObjectType> colliders = this->getCollidingTypes(GameCollisionHandler::explosion_map, obj->getObjectType());
        std::unordered_set<GameObject *> objects_that_were_on_my_current_cell = this->previous_board.getObjectsOnCell(current_location); // potential mid step collisions
        for (GameObject *other : objects_that_were_on_my_current_cell)
        {
            if (obj == other) // never happens because object moved - here for clarity only
                continue;

            if (!updated_board.isObjectOnBoard(other)) // other died last step
                continue;

            if (colliders.find(CollisionObjectTypeUtils::fromGameObjectType(other->getObjectType())) == colliders.end()) // obj, other can't collide
                continue;

            BoardCell other_current_loc = updated_board.getObjectLocation(other).value(); // no need to check optional since object on board

            if (other_current_loc == previous_location &&
                dynamic_cast<MovableObject *>(obj)->getDirection() == DirectionUtils::getOppositeDirection(dynamic_cast<MovableObject *>(other)->getDirection()))
            {
                // TODO: this is ok for now since all moving objects have 1 hp. to ensure only 1 hit per collision later - list mid step colliders and handle at the end
                obj->gotHit(1);
                other->gotHit(1);
            }
        }
    }
}

void GameCollisionHandler::handleEndOfStepCollisionsOnCell(GameBoard &updated_board, const BoardCell &cell) const
{
    std::unordered_set<GameObject *> cell_objects = updated_board.getObjectsOnCell(cell);
    for (auto iter = cell_objects.begin(); iter != cell_objects.end(); ++iter)
    {
        // skip dead objects
        GameObject *obj1 = *iter;
        if (!updated_board.isObjectOnBoard(obj1))
            continue;

        // check for collisions with other objects
        std::unordered_set<CollisionObjectType> collide_with = getCollidingTypes(explosion_map, obj1->getObjectType());
        auto iter2 = iter;
        ++iter2;
        for (; iter2 != cell_objects.end(); ++iter2)
        {
            // skip dead objects
            GameObject *obj2 = *iter2;
            if (!updated_board.isObjectOnBoard(obj2))
                continue;

            // if collision - hit objects by 1
            CollisionObjectType obj2_type = CollisionObjectTypeUtils::fromGameObjectType(obj2->getObjectType());
            if (collide_with.find(obj2_type) != collide_with.end())
            {
                if (updated_board.isObjectOnBoard(obj1)) // test alive again to avoid multiple collisions error (an object that collides with many others)
                    obj1->gotHit(1);

                obj2->gotHit(1);
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
        this->handleEndOfStepCollisionsOnCell(updated_board, cell);
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

const std::unordered_set<CollisionObjectType> GameCollisionHandler::getCollidingTypes(const CollisionMap &collision_map, GameObjectType objType)
{
    auto iter = collision_map.find(CollisionObjectTypeUtils::fromGameObjectType(objType));
    if (iter != collision_map.end())
    {
        return iter->second;
    }
    return {};
}

// === Static Functions === //
bool GameCollisionHandler::isObjectAllowedToStepOn(const GameBoardView &board, GameObjectType obj_type, BoardCell c)
{
    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::prevention_map, board, obj_type, c);
}

bool GameCollisionHandler::canObjectSafelyStepOn(const GameBoardView &board, GameObjectType obj_type, BoardCell c)
{
    if (!GameCollisionHandler::isObjectAllowedToStepOn(board, obj_type, c))
    {
        return false;
    }

    return !GameCollisionHandler::isCollidingOnCell(GameCollisionHandler::explosion_map, board, obj_type, c);
}

bool GameCollisionHandler::isCollidingOnCell(const CollisionMap &collision_map, const GameBoardView &board, GameObjectType obj_type, BoardCell c)
{
    const std::unordered_set<CollisionObjectType> collidors = getCollidingTypes(collision_map, obj_type);
    if (collidors.empty())
        return false;

    auto objects_types_on_cell = board.getObjectsTypesOnCell(c);
    for (GameObjectType object_type : objects_types_on_cell)
    {
        CollisionObjectType t = CollisionObjectTypeUtils::fromGameObjectType(object_type);
        if (collidors.find(t) != collidors.end())
            return true;
    }

    return false;
}