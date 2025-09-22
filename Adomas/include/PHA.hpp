#pragma once

#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstdint>
#include <random>
#include <fstream>
using namespace std;

string float_to_hex16(double inputValue);

string PHA256(const string &inputstring);

string rand_hash(const string &inputString);