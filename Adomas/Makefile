# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always -g -std=c++20 -Iinclude -mconsole -static-libgcc -static-libstdc++

SRC = src/PHA.cpp

test: test_input 

test_input: $(SRC) tests/test_input.cpp
	$(CXX) $(CXXFLAGS) -o build/$@.exe $^

test_complexity: $(SRC) tests/test_complexity.cpp
	$(CXX) $(CXXFLAGS) -o build/$@.exe $^

util: $(SRC) tests/file_creation_util.cpp
	$(CXX) $(CXXFLAGS) -o build/$@.exe $^

clean:
	rm build/*.exe