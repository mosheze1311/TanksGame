@echo off
setlocal enabledelayedexpansion

:: Compiler and flags
set CXX=g++
set CXXFLAGS=-std=c++20 -Wall

:: Target executable
set TARGET=tanks_game.exe

:: Source files
set SRCS=^
BoardFactory\BoardFactory.cpp ^
GameBoard\BoardCell.cpp ^
GameBoard\GameBoard.cpp ^
GameCollisionHandler\GameCollisionHandler.cpp ^
GameManager\GameManager.cpp ^
GameObjects\GameObject.cpp ^
GameObjects\MovableObject.cpp ^
GameObjects\StaticObject.cpp ^
GameObjects\Mine.cpp ^
GameObjects\Shell.cpp ^
GameObjects\Tank.cpp ^
GameObjects\Wall.cpp ^
GamePlayer\GamePlayer.cpp ^
Logger\Logger.cpp ^
main.cpp

:: Compile all source files into object files and then link
echo Compiling and linking...
%CXX% %CXXFLAGS% %SRCS% -o %TARGET%

if %ERRORLEVEL% neq 0 (
    echo Build failed.
    exit /b %ERRORLEVEL%
) else (
    echo Build succeeded. Output: %TARGET%
)