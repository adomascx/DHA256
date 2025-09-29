# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always -g -std=c++20 -Iinclude -mconsole

SRC = src/PHA256.cpp
TEST_SRC = src/tests.cpp

all : test main

test: test_man test_auto


main: $(SRC) tests/main.cpp
	$(CXX) $(CXXFLAGS) -o build/PHA256.exe $^

test_man: $(SRC) $(TEST_SRC) tests/test_man.cpp
	$(CXX) $(CXXFLAGS) -o build/$@.exe $^

test_auto: $(SRC) $(TEST_SRC) tests/test_auto.cpp
	$(CXX) $(CXXFLAGS) -o build/$@.exe $^


clean:
	rm build/*.exe