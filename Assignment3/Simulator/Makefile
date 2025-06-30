# === Compiler and Flags ===
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address

# === Build Folder and Target Executable ===
BUILD_DIR = build
TARGET = simulator_211388913_322330820

# === List of Source Files ===
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

# Optional: automatic clean on make
.PHONY: clean