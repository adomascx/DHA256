#include "PHA.hpp"

string float_to_hex32(double inputValue)
{
    // Atsikratyti minuso zenklo
    inputValue = fabs(inputValue);

    // Normalize inputValue to the range (1, 10) by dividing by the appropriate power of ten
    // (e.g., 0.123 -> 1.23 or 12.3 -> 1.23)
    double normalizedValue = inputValue / pow(10.0, floor(log10(inputValue)));

    // Fold 40 decimal digits into two 64-bit accumulators
    uint64_t digitAccumulator64_1 = 0;
    uint64_t digitAccumulator64_2 = 0;

    // First 20 digits into first accumulator
    for (int i = 0; i < 20; ++i)
    {
        uint64_t d = static_cast<uint64_t>(normalizedValue); // 0..9
        digitAccumulator64_1 = digitAccumulator64_1 * 10u + d;
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }

    // Next 20 digits into second accumulator
    for (int i = 0; i < 20; ++i)
    {
        uint64_t d = static_cast<uint64_t>(normalizedValue); // 0..9
        digitAccumulator64_2 = digitAccumulator64_2 * 10u + d;
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }

    ostringstream returnStream;
    returnStream << hex << uppercase << setfill('0')
                 << setw(16) << digitAccumulator64_1
                 << setw(16) << digitAccumulator64_2;
    return returnStream.str();
}

string PHA256(const string &inputstring)
{
    // Start at coordinates (0,0)
    double x = 0, y = 0;

    // Create "pendulums" of length 1 in a chain
    for (unsigned char a : inputstring)
    {
        x += cos(a);
        y += sin(a);
    }

    ostringstream out;
    out << hex << uppercase << setfill('0')
        << float_to_hex32(x)
        << float_to_hex32(y);

    return out.str(); // 64 hex chars: first 32 = x, last 32 = y
}

