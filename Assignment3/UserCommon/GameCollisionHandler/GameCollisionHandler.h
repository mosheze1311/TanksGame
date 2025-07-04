#pragma once

#include "GameBoardShallowCopy.h"
#include "CollisionObjectType.h"

#include "../GameBoard/GameBoard.h"
#include "../GameObjects/GameObjects.h"
#include "../AbstractGameBoardView/AbstractGameBoardView.h"
#include "../BoardSatelliteView/BoardSatelliteView.h"
#include "../Utils/DirectionUtils.h"

#include <map>
#include <unordered_set>
#include <vector>
namespace UserCommon_211388913_322330820
{

#define CollisionMap std::map<CollisionObjectType, const std::unordered_set<CollisionObjectType>>

    class GameCollisionHandler
    {
        /*
        ===== COLLISION POLICY =====
        Explosion List - if the items are together both will explode/die
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
        // === Attributes === //
        GameBoardShallowCopy previous_board;
        GameBoard &updated_board;
        
        // === Static Members - Collision maps === //
        inline static const CollisionMap explosion_map = {
            {CollisionObjectType::MINE, {CollisionObjectType::TANK}},
            {CollisionObjectType::TANK, {CollisionObjectType::MINE, CollisionObjectType::TANK, CollisionObjectType::SHELL}},
            {CollisionObjectType::SHELL, {CollisionObjectType::SHELL, CollisionObjectType::TANK, CollisionObjectType::WALL}},
            {CollisionObjectType::WALL, {CollisionObjectType::SHELL}}};

        inline static const CollisionMap prevention_map = {
            {CollisionObjectType::MINE, {CollisionObjectType::WALL}},
            {CollisionObjectType::TANK, {CollisionObjectType::WALL}},
            {CollisionObjectType::SHELL, {}},
            {CollisionObjectType::WALL, {CollisionObjectType::TANK, CollisionObjectType::MINE}}};

        // === Static Functions === //
        // check for collisions regarding the collision map
        static bool isCollidingOnCell(const CollisionMap &collision_map, const AbstractGameBoardView &board, GameObjectType obj_type, BoardCell c);

        // explosion list
        static const std::unordered_set<CollisionObjectType> getCollidingTypes(const CollisionMap &collision_map, GameObjectType t);

        // === Functions === //
        // check for collision mid step (went through each other)
        void handleMidStepCollisions();

        // check for collisions after step (objects in the same locaition)
        void handleEndOfStepCollisions();
        void handleEndOfStepCollisionsOnCell(const BoardCell &cell);

        // add new shells to precious board to allow mid-step collision handling
        void positionNewShellsOnPreviousBoard();

    public:
        // === Constructors === //
        GameCollisionHandler(GameBoard &board);
        ~GameCollisionHandler();

        // === Functions === //
        void handleCollisions();

        // === Static Functions === //
        static bool isObjectAllowedToStepOn(const AbstractGameBoardView &board, GameObjectType obj_type, BoardCell c);

        static bool canObjectSafelyStepOn(const AbstractGameBoardView &board, GameObjectType obj_type, BoardCell c);
    };
}