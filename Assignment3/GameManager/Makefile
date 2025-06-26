CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -fPIC
LDFLAGS = -shared -undefined dynamic_lookup

TARGET = GameManager_211388913_322330820.so
SRCS = \
	GameManager.cpp \
	RegisterManager.cpp \
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
	../UserCommon/Utils/GameBoardUtils.cpp \
	../UserCommon/Logger/Logger.cpp

OBJS := $(SRCS)
OBJS := $(patsubst GameManager.cpp, build/GameManager/GameManager.o, $(OBJS))
OBJS := $(patsubst RegisterManager.cpp, build/GameManager/RegisterManager.o, $(OBJS))
OBJS := $(patsubst ../UserCommon/%, build/UserCommon/%, $(OBJS:.cpp=.o))

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS)  $(OBJS) -o $(TARGET)


# Rule to compile GameManager and RegisterManager
build/GameManager/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to compile UserCommon files
build/UserCommon/%.o: ../UserCommon/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f *.l

# Optional: automatic clean on make
.PHONY: clean