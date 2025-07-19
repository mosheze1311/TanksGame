# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address

# === Build Folder and Target Executable ===
BUILD_DIR = build
TARGET = drawer

# === List of Source Files ===
SRCS = \
	main.cpp \
	\
	../../Simulator/MapFileParser/MapFileParser.cpp \
	\
	../GameBoard/BoardCell.cpp \
	../GameBoard/GameBoard.cpp \
	\
	../GameObjects/GameObject.cpp \
	../GameObjects/MovableObject.cpp \
	../GameObjects/Tank.cpp \
	../GameObjects/Shell.cpp \
	../GameObjects/StaticObject.cpp \
	../GameObjects/Wall.cpp \
	../GameObjects/Mine.cpp \
	\
	../BoardSatelliteView/BoardSatelliteView.cpp \
	../GameCollisionHandler/GameCollisionHandler.cpp \
	\
	../AbstractGameBoardView/AbstractGameBoardView.cpp \
	\
	../Logger/Logger.cpp

# === Convert SRCS paths to OBJS paths in build ===
OBJS := $(SRCS)
OBJS := $(patsubst ../%, %, $(OBJS)) # remove ../ before creating build paths
OBJS := $(patsubst %, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))

# === Rule to Create the Executable ===
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# === Compilation Rules ===
$(BUILD_DIR)/%.o: ../%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

	

# === Clean Rules ===
clean:
	rm -r -f $(BUILD_DIR)
	rm -f $(TARGET)
