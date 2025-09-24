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

// ====================== Improved experimental variants ======================
// NOTE: These functions are STILL **NOT** cryptographically secure. They are
// heuristic / educational and keep the original "trig walk -> fold float"
// idea while adding:
//  - Better domain safety (zero / denorm handling)
//  - Additional non‑linear mixing and avalanche style bit diffusion
//  - Slightly richer per‑character dynamics (angle perturbation & chaotic term)
// Output size / external behaviour kept analogous (256-bit style hex string).
// ============================================================================

// Improved 128-bit (32 hex chars) fold of a floating point magnitude.
// Retains the concept of extracting decimal digits but adds robust handling
// and a SplitMix-inspired avalanche to diffuse bits better.
string float_to_hex32_AI(double inputValue);

// Improved "PHA256" style function producing 64 hex chars by combining two
// improved 128-bit folds of the final (x, y) accumulators.
string PHA256_AI(const string &inputstring);