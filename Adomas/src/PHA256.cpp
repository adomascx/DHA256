#include "PHA256.hpp"

string float_to_hex32(double inputValue)
{
    // Atsikratome minuso ženklo
    inputValue = fabs(inputValue);

    // Normalizuojame modulį į [1, 10), kad pirmasis išgaunamas skaitmuo būtų vienetų vietoje
    // Pvz., 12.3 -> 1.23, 0.123 -> 1.23
    double normalizedValue = inputValue / pow(10.0, floor(log10(inputValue)));

    // Išgauname lygiai 40 skaitmenų iš normalizedValue ir sudedame juos į du 64-bit akumuliatorius be ženklo
    uint64_t digitAccumulator64_1 = 0;
    uint64_t digitAccumulator64_2 = 0;

    // Pirmos 20 skaitmenų -> į pirmą akumuliatorių
    for (int i = 0; i < 20; ++i)
    {
        // Paimame pirmą skaitmenį (0..9)
        uint64_t d = static_cast<uint64_t>(normalizedValue);
        // Pastumiame ankstesnius skaitmenis dešimtaine baze ir prijungiame ankstesnįjį
        digitAccumulator64_1 = digitAccumulator64_1 * 10u + d;
        // Atsikratome šio skaitmens ir perkeliame kitą į vienetų vietą
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }

    // Kiti 20 skaitmenų -> į antrą akumuliatorių
    for (int i = 0; i < 20; ++i)
    {
        uint64_t d = static_cast<uint64_t>(normalizedValue);
        digitAccumulator64_2 = digitAccumulator64_2 * 10u + d;
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }

    // Išvedame abu 64 bitų skaičius hex formatu (po 16 simbolių)
    ostringstream returnStream;
    returnStream << hex << uppercase << setfill('0')
                 << setw(16) << digitAccumulator64_1
                 << setw(16) << digitAccumulator64_2;

    return returnStream.str();
}

string PHA256(const string &inputstring)
{

    // Pradėti nuo koordinačių (0,0)
    double x = 0, y = 0;

    for (unsigned char a : inputstring)
    {
        // Kiekvienas simbolis prideda 1 ilgio "švytuoklę" (pendulum) tam tikru kampu, kur kampas = a (radianais).
        x += cos(a);
        y += sin(a);
    }

    // Kiekvieną koordinatę konvertuoti į 32 hex simbolius
    ostringstream out;
    out << hex << uppercase << setfill('0')
        << float_to_hex32(x)  // pirmi 32 šešioliktainiai simboliai iš x
        << float_to_hex32(y); // kiti 32 šešioliktainiai simboliai iš y

    // Iš viso išvedami 64 hex simboliai
    return out.str();
}

// ====================== Improved experimental variants ======================
// Still NOT cryptographically secure. These variants attempt to:
//  * Provide deterministic handling for edge cases (0, infinities, NaNs)
//  * Increase diffusion by (a) varying the step angle per character with
//    lightweight chaotic mixing, (b) introducing cross-coupled updates of x/y
//  * Capture both magnitude and exponent information in the fold function
//  * Apply a SplitMix64-like avalanche on interim 64-bit words
// The high-level idea (accumulate via trig, fold floats to hex) remains.

namespace
{
    // SplitMix64 style mixer for 64-bit diffusion
    inline uint64_t mix64(uint64_t z)
    {
        z += 0x9E3779B97F4A7C15ULL;
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
        return z ^ (z >> 31);
    }

    inline double safe_abs(double v)
    {
        if (std::isnan(v))
            return 0.0; // canonicalize NaN
        if (std::isinf(v))
            return std::numeric_limits<double>::max() / 2.0; // clamp
        return fabs(v);
    }
}

string float_to_hex32_AI(double inputValue)
{
    inputValue = safe_abs(inputValue);

    // Incorporate exponent explicitly so numbers differing only by scale diverge early.
    int exp10 = 0;
    if (inputValue > 0.0)
    {
        exp10 = static_cast<int>(floor(log10(inputValue)));
    }
    else
    {
        // For zero just return deterministic value
        // but still run through machinery to keep length consistent.
        inputValue = 0.0;
    }

    double normalizedValue = (inputValue > 0.0)
                                 ? (inputValue / pow(10.0, exp10))
                                 : 0.0; // in [1,10) ideally when >0

    uint64_t acc1 = 0, acc2 = 0;

    // Extract 24 digits instead of 20 for first half for a bit more entropy
    for (int i = 0; i < 24; ++i)
    {
        uint64_t d = static_cast<uint64_t>(normalizedValue);
        acc1 = acc1 * 11u + d; // use base 11 to alter distribution
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }
    for (int i = 0; i < 24; ++i)
    {
        uint64_t d = static_cast<uint64_t>(normalizedValue);
        acc2 = acc2 * 13u + d; // different base
        normalizedValue = (normalizedValue - static_cast<double>(d)) * 10.0;
    }

    // Fold exponent
    uint64_t expComponent = static_cast<uint64_t>(static_cast<int64_t>(exp10)) & 0xFFFFULL;
    acc1 ^= (expComponent << 48);
    acc2 ^= (expComponent << 32);

    // Mix with avalanche
    acc1 = mix64(acc1);
    acc2 = mix64(acc2 ^ acc1);

    ostringstream oss;
    oss << hex << uppercase << setfill('0')
        << setw(16) << acc1
        << setw(16) << acc2;
    return oss.str();
}

string PHA256_AI(const string &inputstring)
{
    // Use two accumulators; start with irrational seeds to break symmetry.
    double x = 0.31830988618379067154; // 1/pi
    double y = 0.41421356237309504880; // sqrt(2)-1
    double z = 0.23205080756887729352; // sqrt(3)/3

    // Additional scalar to perturb phase
    double phase = 0.0;

    // Iterate over bytes
    for (unsigned char c : inputstring)
    {
        // Derive an angle influenced by character and evolving phase
        double angle = (static_cast<double>(c) + 37.0 * phase + z * 911.0);
        double s = sin(angle);
        double co = cos(angle);

        // Cross-coupled nonlinear updates
        double nx = x + co + 0.5 * s * (y - z);
        double ny = y + s + 0.5 * co * (z - x);
        double nz = z + (s * co) * 0.75 + 0.125 * (x - y);

        x = nx;
        y = ny;
        z = nz;

        // Update phase with a bounded chaotic map (logistic inspired)
        phase += (static_cast<double>(c) * 0.0078125) + (s * co * 0.03125);
        phase = fmod(phase, 1024.0);
    }

    // Derive two composite magnitudes to fold; include interactions
    double mag1 = safe_abs(x + 0.61803398874989484820 * y + 0.5 * z);
    double mag2 = safe_abs(y - 0.70710678118654752440 * x + 1.25 * z);

    // Introduce a small stabilization if zero
    if (mag1 == 0.0)
        mag1 = 1e-300;
    if (mag2 == 0.0)
        mag2 = 1e-300;

    ostringstream out;
    out << hex << uppercase << setfill('0')
        << float_to_hex32_AI(mag1)
        << float_to_hex32_AI(mag2);
    return out.str();
}