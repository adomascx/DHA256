#include "PHA.hpp"

string float_to_hex16(double inputValue)
{
    // Remove sign so −x and x map the same
    inputValue = fabs(inputValue);

    // Normalize to decimal significand in [1, 10)
    int decimalExponent = static_cast<int>(floor(log10(inputValue)));
    double decimalSignificand = inputValue / pow(10.0, decimalExponent);

    // Fold 20 decimal digits into a 64-bit accumulator
    uint64_t digitAccumulator64 = 0;
    const int digitsToFold = 20;

    for (int digitIndex = 0; digitIndex < digitsToFold; ++digitIndex)
    {
        double extractedDigit;
        decimalSignificand = modf(decimalSignificand, &extractedDigit); // extractedDigit ∈ {0..9}
        digitAccumulator64 = digitAccumulator64 * 10u + static_cast<uint64_t>(extractedDigit);
        decimalSignificand *= 10.0;
    }

    ostringstream hexStream;
    hexStream << hex << nouppercase << setfill('0') << setw(16) << digitAccumulator64;
    return hexStream.str();
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
    out << hex << nouppercase << setfill('0')
        << float_to_hex16(x)
        << float_to_hex16(y);

    return out.str(); // 32 hex chars: first 16 = x, last 16 = y
}

string rand_hash(const string &inputString)
{
    int sum = 0;
    for (char c : inputString)
    {
        sum += static_cast<unsigned char>(c);
    }

    srand(sum);

    // Generate 64 hex characters
    ostringstream out;
    out << hex;
    for (int i = 0; i < 64; i++)
    {
        int randomValue = rand() % 16; // Generate 0-15
        out << randomValue;
    }
    return out.str();
}