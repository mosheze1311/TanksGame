#pragma once
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"

#include <unordered_set>
#include <map>
#include <vector>

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
    GameBoard previous_board;
    static std::map<GameObjectType, std::map<GameObjectType, int>> explosion_map;
    static std::map<GameObjectType, std::map<GameObjectType, int>> prevention_map;

    //=== Functions ===
    // check for collision mid step (went through each other)
    void handleMidStepCollisions(GameBoard &updated_board);
    
    // check for collisions after step (objects in the same locaition)
    void handleEndOfStepCollisions(GameBoard &updated_board);

    // explosion list
    std::map<GameObjectType, int> getExplosionList(GameObjectType t);

    // check for collisions regarding the collision map
    static bool isCollision(std::map<GameObjectType, std::map<GameObjectType, int>> collision_map, const GameBoard& board, GameObjectType obj_type, BoardCell c);

public:
    //=== Constructors ===
    GameCollisionHandler(GameBoard &board);
    ~GameCollisionHandler();

    //=== Functions ===
    void handleCollisions(GameBoard& updatedBoard);

    static bool isObjectAllowedToStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c);

    static bool canObjectSafelyStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c);
};
