cmake_minimum_required(VERSION 3.30)
project(TanksGame)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(BoardFactory)
include_directories(GameBoard)
include_directories(GameCollisionHandler)
include_directories(GameManager)
include_directories(GameDrawer)
include_directories(GameObjects)
include_directories(GamePlayer)
include_directories(Logger)

add_executable(TanksGame
    BoardFactory/BoardFactory.cpp
    BoardFactory/BoardFactory.h
    GameBoard/GameBoard.cpp
    GameBoard/BoardCell.cpp
    GameBoard/GameBoard.h
    GameCollisionHandler/GameCollisionHandler.cpp
    GameCollisionHandler/GameCollisionHandler.h
    GameManager/GameManager.cpp
    GameManager/GameManager.h
    GameObjects/Direction.h
    GameObjects/GameObject.cpp
    GameObjects/GameObjects.h
    GameObjects/TankAction.h
    GameObjects/GameObjectType.h
    GameObjects/Mine.cpp
    GameObjects/MovableObject.cpp
    GameObjects/Shell.cpp
    GameObjects/StaticObject.cpp
    GameObjects/Tank.cpp
    GameObjects/Wall.cpp
    GamePlayer/GamePlayer.cpp
    GamePlayer/GamePlayer.h
    Logger/Logger.cpp
    Logger/Logger.h
    GameDrawer/GameDrawer.h
    main.cpp)
