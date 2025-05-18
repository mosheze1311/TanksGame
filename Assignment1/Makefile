# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address

# Target executable
TARGET = tanks_game

# List of source files
SRCS = BoardFactory/BoardFactory.cpp GameBoard/BoardCell.cpp GameBoard/GameBoard.cpp GameCollisionHandler/GameCollisionHandler.cpp GameManager/GameManager.cpp GameObjects/GameObject.cpp GameObjects/MovableObject.cpp GameObjects/StaticObject.cpp GameObjects/Mine.cpp GameObjects/Shell.cpp GameObjects/Tank.cpp GameObjects/Wall.cpp GamePlayer/GamePlayer.cpp GamePlayer/PlayerUtils.cpp Logger/Logger.cpp main.cpp

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