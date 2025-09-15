# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always -g -std=c++20 -Iinclude -mconsole -static-libgcc -static-libstdc++

SRC = src/PHA.cpp tests/test.cpp
BIN_DIR = build
TARGET = $(BIN_DIR)/test.exe

.PHONY: all test clean main_

all: $(TARGET)

# Create build dir in a portable way (Windows cmd or POSIX shell)
$(BIN_DIR):
ifeq ($(OS),Windows_NT)
	if not exist $(BIN_DIR) mkdir $(BIN_DIR)
else
	mkdir -p $(BIN_DIR)
endif

$(TARGET): $(BIN_DIR) $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

test: $(TARGET)
	@echo "Built $(TARGET)"

main_: $(TARGET)

clean:
ifeq ($(OS),Windows_NT)
	if exist $(BIN_DIR) rmdir /s /q $(BIN_DIR)
	if exist test.exe del /q test.exe
else
	rm -rf $(BIN_DIR) test.exe
endif