# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address -fPIC
LDFLAGS = -shared -undefined dynamic_lookup

# === Build Folder and Target Executable ===
BUILD_DIR = build
TARGET = Algorithm_211388913_322330820_Aggressive.so

# === List of Source Files ===
SRCS = \
	RegisterAlgorithm.cpp \
	\
	TankAlgorithm/AbstractTankAlgorithm.cpp \
	TankAlgorithm/AggressiveTankAlgorithm.cpp \
	TankAlgorithm/PassiveTankAlgorithm.cpp \
	Tactics/Tactic.cpp \
	Formation/Formation.cpp \
	BattleInfo/BattleInfoAgent.cpp \
	SatelliteAnalyticsView/SatelliteAnalyticsView.cpp \
	Players/AbstractPlayer.cpp \
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
	../UserCommon/BoardSatelliteView/BoardSatelliteView.cpp \
	../UserCommon/GameCollisionHandler/GameCollisionHandler.cpp \
	\
	../UserCommon/AbstractGameBoardView/AbstractGameBoardView.cpp \
	\
	../UserCommon/Logger/Logger.cpp

# === Convert SRCS paths to OBJS paths in build ===
OBJS := $(SRCS)
OBJS := $(patsubst ../%, %, $(OBJS)) # remove ../ before creating build paths
OBJS := $(patsubst %, $(BUILD_DIR)/%, $(OBJS:.cpp=.o))

# === Rule to Create the Executable ===
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o $(TARGET)

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
