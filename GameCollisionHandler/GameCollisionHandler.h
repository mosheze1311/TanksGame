#pragma once
#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"

#include <unordered_set>
#include <map>
#include <vector>
#

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

public:

    GameCollisionHandler(GameBoard& board);
    ~GameCollisionHandler();

    void handleCollisions(GameBoard& updatedBoard);

    // TODO: merge replication to a helper function
    static bool isObjectAllowedToStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c){
        auto objects_on_cell = board.getObjectsOnCell(c);
        map<GameObjectType, int> counters;
        for (auto obj : objects_on_cell)
        {
            counters[obj->getObjectType()] += 1;
        }

        for (auto count_pair : counters)
        {
            if (GameCollisionHandler::prevention_map[obj_type][count_pair.first] <= count_pair.second)
            {
                return false;
            }
        }
        return true;
    }

    static bool canObjectSafelyStepOn(const GameBoard &board, GameObjectType obj_type, BoardCell c)
    {
        if (!GameCollisionHandler::isObjectAllowedToStepOn(board,obj_type, c)){
            return false;
        }

        auto objects_on_cell = board.getObjectsOnCell(c);
        map<GameObjectType, int> counters;
        for (auto obj : objects_on_cell)
        {
            counters[obj->getObjectType()] += 1;
        }

        for (auto count_pair : counters)
        {
            if (GameCollisionHandler::explosion_map[obj_type][count_pair.first] <= count_pair.second)
            {
                return false;
            }
        }
        return true;
    }
};
