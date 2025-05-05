# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20

# Target executable
TARGET = random_input_generator

# List of source files
SRCS = generate_random_input_file.cpp

# Object files (replace .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Rule to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Rule to create object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Optional: automatic clean on make
.PHONY: clean