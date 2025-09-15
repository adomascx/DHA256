#include "PHA.hpp"

string PHA256(const string &inputstring)
{
    // Start at coordinates (0,0)
    double x = 0, y = 0;

    // Walk: angle = byte (radians), step by 1
    for (unsigned char b : inputstring)
    {
        double a = static_cast<double>(b);
        x += cos(a);
        y += sin(a);
    }

    // Remove decimal point and truncate toward zero
    const double SCALE = 1e15;
    int64_t xi = static_cast<int64_t>(trunc(x * SCALE));
    int64_t yi = static_cast<int64_t>(trunc(y * SCALE));

    // Two's-complement pack + exact 16-hex chars each
    uint64_t ux = static_cast<uint64_t>(xi);
    uint64_t uy = static_cast<uint64_t>(yi);

    ostringstream out;
    out << hex << nouppercase << setfill('0')
        << setw(16) << static_cast<unsigned long long>(ux)
        << setw(16) << static_cast<unsigned long long>(uy);

    return out.str(); // 32 hex chars: first 16 = x, last 16 = y
}
