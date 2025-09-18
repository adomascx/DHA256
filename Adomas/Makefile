# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always -g -std=c++20 -Iinclude -mconsole -static-libgcc -static-libstdc++

SRC = src/PHA.cpp tests/test.cpp
TARGET = test.exe

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(SRC)

main: $(TARGET)

clean:
	rm *.exe