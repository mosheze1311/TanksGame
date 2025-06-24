# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address

# Target executable
TARGET = simulator_211388913_322330820

# List of source files
SRCS = \
    main.cpp \
    Simulator.cpp \
	MapFileParser/MapFileParser.cpp \
	Registrar/Registrations.cpp \
	\
	../UserCommon/GameBoard/BoardCell.cpp \
	../UserCommon/GameBoard/GameBoard.cpp \
	\
	../UserCommon/GameObjects/GameObject.cpp \
	../UserCommon/GameObjects/MovableObject.cpp \
	../UserCommon/GameObjects/Tank.cpp \
	../UserCommon/GameObjects/Shell.cpp \
	../UserCommon/GameObjects/StaticObject.cpp \
	../UserCommon/GameObjects/Wall.cpp \
	../UserCommon/GameObjects/Mine.cpp \
	\
	../UserCommon/GameCollisionHandler/GameCollisionHandler.cpp \
	\
	../UserCommon/BoardSatelliteView/BoardSatelliteView.cpp \
	../UserCommon/Utils/GameBoardUtils.cpp \
	../UserCommon/Logger/Logger.cpp

# Convert source paths to object paths in build/
OBJS := $(patsubst ../%, build/%, $(SRCS:.cpp=.o))

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Rule to compile each .cpp into its corresponding .o in build/
build/%.o: ../%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *.l

# Optional: automatic clean on make
.PHONY: clean