#pragma once
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "GameBoardShallowCopy.h"
#include "CollisionObjectType.h"
#include "../GameBoardUtils/GameBoardUtils.h"

#include <unordered_set>
#include <map>
#include <vector>

#define CollisionMap std::map<CollisionObjectType, const unordered_set<CollisionObjectType>>

class GameCollisionHandler
{
    /*
    ===== COLLISION POLICY =====
    Explosion List - if the items are together both will explode/dye
    Prevention List - items can't be on the same cell. will cancel the placement.

    ---Explosion List---
    MINE - TANK
    TANK - MINE, TANK, SHELL
    SHELL - SHELL, TANK, WALL
    WALL - SHELL

    ---Prevention List---
    MINE - WALL
    SHELL -
    TANK - WALL
    WALL - TANK, MINE
    */ 

private:
    //=== Attributes ===
    GameBoardShallowCopy previous_board;
    static const CollisionMap explosion_map;
    static const CollisionMap prevention_map;

    //=== Static Functions ===
    // check for collisions regarding the collision map
    static bool isCollidingOnCell(CollisionMap collision_map, const GameBoard &board, GameObjectType obj_type, BoardCell c);

    // explosion list
    static const unordered_set<CollisionObjectType> getCollidingTypes(CollisionMap collision_map, GameObjectType t);

    //=== Functions ===
    // check for collision mid step (went through each other)
    void handleMidStepCollisions(GameBoard &updated_board) const;
    
    // check for collisions after step (objects in the same locaition)
    void handleEndOfStepCollisions(GameBoard &updated_board) const;

    // add new shells to precious board to allow mid-step collision handling
    void positionNewShellsOnPreviousBoard(const GameBoard &updated_board);

    public :
    //=== Constructors ===
    GameCollisionHandler(GameBoard &board);
    ~GameCollisionHandler();

    //=== Functions ===
    void handleCollisions(GameBoard& updatedBoard);

    //=== Static Functions ===
    static bool isObjectAllowedToStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c);

    static bool canObjectSafelyStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c);
};