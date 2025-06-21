# Compiler and flags
CXX := g++
# Automatically find all subdirectories in ../ and add them as include paths
INCLUDE_DIRS := $(shell find ../ -type d)
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -fsanitize=address $(addprefix -I, $(INCLUDE_DIRS)) -DTEST_MODE

# Find all .cpp files in ../, but exclude main.cpp and anything inside ../ExtraMainDir
SRCS := $(shell find ../ -name "*.cpp" ! -name "main.cpp" ! -path "../ScriptsAndHelpers/*")

# Convert source paths to object paths in build/
OBJS := $(patsubst ../%, build/%, $(SRCS:.cpp=.o))

# Target binary
TARGET := game_tester

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Rule to compile each .cpp into its corresponding .o in build/
build/%.o: ../%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -rf build $(TARGET)

.PHONY: all clean