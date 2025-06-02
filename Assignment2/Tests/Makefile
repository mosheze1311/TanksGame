# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address

# Target executable
TARGET = tanks_game

# List of source files
SRCS = \
    ../GameManager/GameManager.cpp \
    ../Players/AbstractPlayer.cpp \
	../TankAlgorithm/AbstractTankAlgorithm.cpp \
	../TankAlgorithm/AggressiveTankAlgorithm.cpp \
	../TankAlgorithm/PassiveTankAlgorithm.cpp \
	../BattleInfo/BattleInfoAgent.cpp \
	../SatelliteAnalyticsView/SatelliteAnalyticsView.cpp \
    ../GameBoardInitializer/GameBoardInitializer.cpp \
    ../GameBoard/BoardCell.cpp \
    ../GameBoard/GameBoard.cpp \
    ../BoardSatelliteView/BoardSatelliteView.cpp \
    ../GameCollisionHandler/GameCollisionHandler.cpp \
    ../GameObjects/GameObject.cpp \
    ../GameObjects/MovableObject.cpp \
    ../GameObjects/StaticObject.cpp \
    ../GameObjects/Mine.cpp \
    ../GameObjects/Shell.cpp \
    ../GameObjects/Tank.cpp \
    ../GameObjects/Wall.cpp \
    ../Utils/GameBoardUtils.cpp \
    ../Logger/Logger.cpp \
    main_test.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to create object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *.log

# Optional: automatic clean on make
.PHONY: clean