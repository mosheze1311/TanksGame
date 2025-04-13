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
    static std::map<GameObjectType, std::unordered_set<GameObjectType>> explosion_map;

    //=== Functions ===
    // check for collision mid step (went through each other)
    void handleMidStepCollisions(GameBoard &updated_board);
    
    // check for collisions after step (objects in the same locaition)
    void handleEndOfStepCollisions(GameBoard &updated_board);

    // explosion list
    std::unordered_set<GameObjectType> getExplosionList(GameObjectType t);

public:
    GameCollisionHandler(GameBoard& board);
    ~GameCollisionHandler();

    void handleCollisions(GameBoard& updatedBoard);
};

GameCollisionHandler::GameCollisionHandler(GameBoard &board) : previous_board(board) {}

GameCollisionHandler::~GameCollisionHandler()
{
}

void GameCollisionHandler::handleCollisions(GameBoard &updated_board) {
    this->handleMidStepCollisions(updated_board);
    this->handleEndOfStepCollisions(updated_board);

    this->previous_board = updated_board;
};


void GameCollisionHandler::handleMidStepCollisions(GameBoard &updated_board){
    // a mid step collision occurs when two objects switch lcations between steps.
    for (GameObject* obj : updated_board.getAllGameObjects()){
        unordered_set<GameObjectType> should_expload = getExplosionList(obj->getObjectType());

        optional<BoardCell> opt_current_loc = updated_board.getObjectLocation(obj);
        optional<BoardCell> opt_previous_loc = previous_board.getObjectLocation(obj);
        if (!opt_current_loc || !opt_previous_loc) // should not happen
            continue;
        
        BoardCell current_location = *opt_current_loc;
        BoardCell previous_location = *opt_previous_loc;
        unordered_set<GameObject *> objects_that_were_on_my_current_cell = this->previous_board.getObjectsOnCell(current_location);
        for (GameObject* other: objects_that_were_on_my_current_cell){
            if (should_expload.find(other->getObjectType()) == should_expload.end())
                continue;

            optional<BoardCell> opt_other_current_loc = updated_board.getObjectLocation(other);
            if(!opt_other_current_loc)
                continue;
            
            BoardCell other_current_loc = *opt_other_current_loc;
            if (other_current_loc == previous_location){
                updated_board.removeObject(obj);
                updated_board.removeObject(other);
            }
        }
    }
}
void GameCollisionHandler::handleEndOfStepCollisions(GameBoard &updated_board) {
    // an end of step collision occurs when two objects are on the same location at the end of the step.
    for (BoardCell& cell : updated_board.getOccupiedCells()){
        unordered_set<GameObjectType> exploding_objects;
        unordered_set<GameObject *> cell_objects = updated_board.getObjectsOnCell(cell);

        for (GameObject *obj : cell_objects)
        {
            unordered_set<GameObjectType> should_expload = getExplosionList(obj->getObjectType());

            exploding_objects.insert(should_expload.begin(), should_expload.end());
        }

        for (GameObject *obj : cell_objects)
        {
            if(exploding_objects.find(obj->getObjectType()) != exploding_objects.end()){
                updated_board.removeObject(obj);
            }
        }
    }
}
