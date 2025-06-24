

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

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)