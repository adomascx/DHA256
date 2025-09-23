# Makefile for Blockchain Hash project

# Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O3

# Directories
BIN_DIR = bin

# Binary name
TARGET = $(BIN_DIR)/hash

# Source files
SOURCES = main.cpp functions.cpp

# Header files
HEADERS = header.h

# Object files
OBJECTS = $(SOURCES:%.cpp=$(BIN_DIR)/%.o)

# Default rule
all: $(BIN_DIR) $(TARGET)

# Create bin directory
$(BIN_DIR):
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)

# Rule to make the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule for object files
$(BIN_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean rule
clean:
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)

# Phony targets
.PHONY: all clean