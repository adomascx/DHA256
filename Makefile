# Compilation variables
CXX = g++
CXXFLAGS = -Wall -Wextra -fdiagnostics-color=always -g -std=c++20 -Iinclude -mconsole -static-libgcc -static-libstdc++

make: 
	$(CXX) $(CXXFLAGS) -o dha.exe *.cpp