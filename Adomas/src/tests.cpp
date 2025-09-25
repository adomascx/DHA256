#include "tests.hpp"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <ctime>
#include <filesystem>
#include <limits>
#include <iomanip>
#include <numeric>
#include <random>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;

namespace
{
    mt19937_64 &rng()
    {
        static random_device rd;
        static mt19937_64 engine(rd());
        return engine;
    }

    char random_alphabet()
    {
        static const string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
        uniform_int_distribution<size_t> dist(0, alphabet.size() - 1);
        return alphabet[dist(rng())];
    }

    string random_string(size_t length)
    {
        if (length == 0)
        {
            return string();
        }

        string result;
        result.reserve(length);
        for (size_t i = 0; i < length; ++i)
        {
            result.push_back(random_alphabet());
        }
        return result;
    }

    size_t popcount4(int value)
    {
        static const int table[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
        return static_cast<size_t>(table[value & 0xF]);
    }

    int hex_char_to_int(char c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';
        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');
        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'a');
        return 0;
    }

    pair<string, string> single_char_difference_pair(size_t length)
    {
        if (length == 0)
        {
            return {string("A"), string("B")};
        }

        string base = random_string(length);
        string modified = base;
        uniform_int_distribution<size_t> posDist(0, length - 1);
        size_t pos = posDist(rng());
        char original = base[pos];
        char replacement = original;
        while (replacement == original)
        {
            replacement = random_alphabet();
        }
        modified[pos] = replacement;
        return {base, modified};
    }

    fs::path locate_tests_file_internal(const string &name)
    {
        vector<fs::path> candidates = {
            fs::current_path() / name,
            fs::current_path() / "tests" / name,
            fs::current_path().parent_path() / "tests" / name,
            fs::current_path().parent_path() / name
        };

        for (const auto &candidate : candidates)
        {
            if (!candidate.empty() && fs::exists(candidate))
            {
                return candidate;
            }
        }

        throw runtime_error("Failed to locate required file: " + name);
    }

    vector<string> read_file_lines(const fs::path &path)
    {
        ifstream input(path);
        if (!input.is_open())
        {
            throw runtime_error("Unable to open file: " + path.string());
        }

        vector<string> lines;
        string line;
        while (getline(input, line))
        {
            lines.push_back(line);
        }
        return lines;
    }

    string join_lines(const vector<string> &lines, size_t count)
    {
        if (lines.empty() || count == 0)
        {
            return string();
        }

        count = min(count, lines.size());
        ostringstream oss;
        for (size_t i = 0; i < count; ++i)
        {
            if (i != 0)
            {
                oss << '\n';
            }
            oss << lines[i];
        }
        return oss.str();
    }

    string run_nojus_hash_impl(const string &input)
    {
        int seed = 2671109;
        for (unsigned char c : input)
        {
            seed += static_cast<int>(c);
        }

        srand(seed);
        uint32_t value = static_cast<uint32_t>(rand());

        for (unsigned char c : input)
        {
            uint32_t byteValue = static_cast<uint32_t>(c);
            int randomShift = rand() % 8;
            uint32_t rotatedValue;
            if (rand() % 2 == 0)
            {
                rotatedValue = (byteValue << randomShift) | (byteValue >> (8 - randomShift));
            }
            else
            {
                rotatedValue = (byteValue >> randomShift) | (byteValue << (8 - randomShift));
            }

            value ^= rotatedValue;
            value = (value << 5) | (value >> 27);
            value += (value >> 2);
        }

        uint32_t part1 = value;
        uint32_t part2 = ~value;
        uint32_t part3 = value ^ 0xFFFFu;
        uint32_t part4 = value * 3u;

        ostringstream hashStream;
        hashStream << hex << nouppercase << setfill('0')
                   << setw(8) << part1
                   << setw(8) << part2
                   << setw(8) << part3
                   << setw(8) << part4;
        return hashStream.str();
    }

    inline uint32_t rotr(uint32_t value, uint32_t shift)
    {
        return (value >> shift) | (value << (32 - shift));
    }

    inline uint32_t sigma0(uint32_t value)
    {
        return rotr(value, 7) ^ rotr(value, 18) ^ (value >> 3);
    }

    inline uint32_t sigma1(uint32_t value)
    {
        return rotr(value, 17) ^ rotr(value, 19) ^ (value >> 10);
    }

    inline uint32_t Sigma0(uint32_t value)
    {
        return rotr(value, 2) ^ rotr(value, 13) ^ rotr(value, 22);
    }

    inline uint32_t Sigma1(uint32_t value)
    {
        return rotr(value, 6) ^ rotr(value, 11) ^ rotr(value, 25);
    }

    inline uint32_t choose_bits(uint32_t x, uint32_t y, uint32_t z)
    {
        return (x & y) ^ (~x & z);
    }

    inline uint32_t majority(uint32_t x, uint32_t y, uint32_t z)
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    constexpr array<uint32_t, 64> SHA256_K = {
        0x428a2f98u, 0x71374491u, 0xb5c0fbcfu, 0xe9b5dba5u,
        0x3956c25bu, 0x59f111f1u, 0x923f82a4u, 0xab1c5ed5u,
        0xd807aa98u, 0x12835b01u, 0x243185beu, 0x550c7dc3u,
        0x72be5d74u, 0x80deb1feu, 0x9bdc06a7u, 0xc19bf174u,
        0xe49b69c1u, 0xefbe4786u, 0x0fc19dc6u, 0x240ca1ccu,
        0x2de92c6fu, 0x4a7484aau, 0x5cb0a9dcu, 0x76f988dau,
        0x983e5152u, 0xa831c66du, 0xb00327c8u, 0xbf597fc7u,
        0xc6e00bf3u, 0xd5a79147u, 0x06ca6351u, 0x14292967u,
        0x27b70a85u, 0x2e1b2138u, 0x4d2c6dfcu, 0x53380d13u,
        0x650a7354u, 0x766a0abbu, 0x81c2c92eu, 0x92722c85u,
        0xa2bfe8a1u, 0xa81a664bu, 0xc24b8b70u, 0xc76c51a3u,
        0xd192e819u, 0xd6990624u, 0xf40e3585u, 0x106aa070u,
        0x19a4c116u, 0x1e376c08u, 0x2748774cu, 0x34b0bcb5u,
        0x391c0cb3u, 0x4ed8aa4au, 0x5b9cca4fu, 0x682e6ff3u,
        0x748f82eeu, 0x78a5636fu, 0x84c87814u, 0x8cc70208u,
        0x90befffau, 0xa4506cebu, 0xbef9a3f7u, 0xc67178f2u};

    constexpr array<uint32_t, 8> SHA256_INITIAL = {
        0x6a09e667u, 0xbb67ae85u, 0x3c6ef372u, 0xa54ff53au,
        0x510e527fu, 0x9b05688cu, 0x1f83d9abu, 0x5be0cd19u};
}

void file_creation_tool(int toolNum, ostream &outStream)
{
    srand(static_cast<unsigned>(time(nullptr)));

    switch (toolNum)
    {
    case '1':
    {
        outStream << char('a' + rand() % 26);
        break;
    }
    case '2':
    {
        string s;
        s.reserve(2000);
        for (int i = 0; i < 2000; ++i)
        {
            s += 'a' + rand() % 26;
        }
        outStream << s;
        break;
    }
    case '3':
    {
        string s;
        s.reserve(2000);
        for (int i = 0; i < 2000; ++i)
        {
            s += 'a' + rand() % 26;
        }
        string s2 = s;
        s2.back() = char('a' + rand() % 26);
        outStream << s << '\n'
                  << s2;
        break;
    }
    case '4':
        break;
    default:
        break;
    }
}

string sha256_hex(const string &input)
{
    array<uint32_t, 8> state = SHA256_INITIAL;

    vector<uint8_t> data(input.begin(), input.end());
    uint64_t bitLength = static_cast<uint64_t>(data.size()) * 8ull;

    data.push_back(0x80u);
    while ((data.size() % 64u) != 56u)
    {
        data.push_back(0x00u);
    }

    for (int shift = 7; shift >= 0; --shift)
    {
        data.push_back(static_cast<uint8_t>((bitLength >> (shift * 8)) & 0xFFu));
    }

    array<uint32_t, 64> schedule{};
    for (size_t offset = 0; offset < data.size(); offset += 64)
    {
        const uint8_t *chunk = data.data() + offset;

        for (size_t i = 0; i < 16; ++i)
        {
            schedule[i] = (static_cast<uint32_t>(chunk[i * 4]) << 24) |
                          (static_cast<uint32_t>(chunk[i * 4 + 1]) << 16) |
                          (static_cast<uint32_t>(chunk[i * 4 + 2]) << 8) |
                          (static_cast<uint32_t>(chunk[i * 4 + 3]));
        }
        for (size_t i = 16; i < 64; ++i)
        {
            schedule[i] = sigma1(schedule[i - 2]) + schedule[i - 7] + sigma0(schedule[i - 15]) + schedule[i - 16];
        }

        uint32_t a = state[0];
        uint32_t b = state[1];
        uint32_t c = state[2];
        uint32_t d = state[3];
        uint32_t e = state[4];
        uint32_t f = state[5];
        uint32_t g = state[6];
        uint32_t h = state[7];

        for (size_t i = 0; i < 64; ++i)
        {
            uint32_t temp1 = h + Sigma1(e) + choose_bits(e, f, g) + SHA256_K[i] + schedule[i];
            uint32_t temp2 = Sigma0(a) + majority(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        state[0] += a;
        state[1] += b;
        state[2] += c;
        state[3] += d;
        state[4] += e;
        state[5] += f;
        state[6] += g;
        state[7] += h;
    }

    ostringstream result;
    result << hex << nouppercase << setfill('0');
    for (uint32_t word : state)
    {
        result << setw(8) << word;
    }
    return result.str();
}

HashAlgorithm resolve_algorithm_choice(char choice)
{
    switch (choice)
    {
    case '2':
        return HashAlgorithm::PHA256_AI;
    case '3':
        return HashAlgorithm::Nojus;
    case '4':
        return HashAlgorithm::SHA256;
    default:
        return HashAlgorithm::PHA256_Basic;
    }
}

string hash_algorithm_name(HashAlgorithm algorithm)
{
    switch (algorithm)
    {
    case HashAlgorithm::PHA256_AI:
        return "PHA256_AI";
    case HashAlgorithm::Nojus:
        return "NojusHash";
    case HashAlgorithm::SHA256:
        return "SHA256";
    default:
        return "PHA256";
    }
}

string run_hash(HashAlgorithm algorithm, const string &input)
{
    switch (algorithm)
    {
    case HashAlgorithm::PHA256_AI:
        return PHA256_AI(input);
    case HashAlgorithm::Nojus:
        return run_nojus_hash_impl(input);
    case HashAlgorithm::SHA256:
        return sha256_hex(input);
    default:
        return PHA256(input);
    }
}

size_t expected_hash_length(HashAlgorithm algorithm)
{
    switch (algorithm)
    {
    case HashAlgorithm::Nojus:
        return 32;
    case HashAlgorithm::SHA256:
        return 64;
    default:
        return 64;
    }
}

bool validate_output_length(HashAlgorithm algorithm, const vector<string> &inputs)
{
    for (const auto &input : inputs)
    {
        const string hash = run_hash(algorithm, input);
        if (hash.size() != expected_hash_length(algorithm))
        {
            return false;
        }
    }
    return true;
}

bool validate_determinism(HashAlgorithm algorithm, const vector<string> &inputs)
{
    for (const auto &input : inputs)
    {
        const string first = run_hash(algorithm, input);
        const string second = run_hash(algorithm, input);
        if (first != second)
        {
            return false;
        }
    }
    return true;
}

vector<string> default_test_inputs()
{
    return {
        "",
        "a",
        "Hello, world!",
        random_string(32),
        random_string(128),
        string(1024, 'x'),
        "The quick brown fox jumps over the lazy dog"
    };
}

vector<TimingResult> perform_konstitucija_benchmark(HashAlgorithm algorithm, int repetitions)
{
    if (repetitions <= 0)
    {
        repetitions = 1;
    }

    const fs::path filePath = locate_tests_file_internal("konstitucija.txt");
    const vector<string> lines = read_file_lines(filePath);

    vector<size_t> lineCounts;
    for (int power = 0; power <= 9; ++power)
    {
        size_t linesToRead = static_cast<size_t>(1) << power;
        if (linesToRead > lines.size())
        {
            break;
        }
        lineCounts.push_back(linesToRead);
    }
    if (lineCounts.empty())
    {
        lineCounts.push_back(lines.size());
    }

    vector<string> preparedInputs;
    preparedInputs.reserve(lineCounts.size());
    for (size_t count : lineCounts)
    {
        preparedInputs.push_back(join_lines(lines, count));
    }

    vector<double> accumulators(lineCounts.size(), 0.0);

    for (int run = 0; run < repetitions; ++run)
    {
        for (size_t idx = 0; idx < preparedInputs.size(); ++idx)
        {
            const auto &input = preparedInputs[idx];
            const auto start = steady_clock::now();
            volatile string hash = run_hash(algorithm, input);
            (void)hash;
            const auto end = steady_clock::now();
            accumulators[idx] += duration<double, milli>(end - start).count();
        }
    }

    vector<TimingResult> results;
    results.reserve(lineCounts.size());
    for (size_t idx = 0; idx < lineCounts.size(); ++idx)
    {
        results.push_back({lineCounts[idx], accumulators[idx] / repetitions});
    }
    return results;
}

vector<CollisionSummary> perform_collision_benchmark(HashAlgorithm algorithm, const BenchmarkConfig &config)
{
    size_t pairsPerLength = max<size_t>(1, config.collisionPairsPerLength);

    vector<CollisionSummary> summaries;
    summaries.reserve(config.collisionInputLengths.size());

    for (size_t length : config.collisionInputLengths)
    {
        size_t collisionCount = 0;
        unordered_map<string, size_t> collisionFrequencies;

        for (size_t i = 0; i < pairsPerLength; ++i)
        {
            const string first = random_string(length);
            const string second = random_string(length);

            const string hashA = run_hash(algorithm, first);
            const string hashB = run_hash(algorithm, second);

            if (hashA == hashB)
            {
                ++collisionCount;
                ++collisionFrequencies[hashA];
            }
        }

        double rate = static_cast<double>(collisionCount) / static_cast<double>(pairsPerLength);
        summaries.push_back({length, pairsPerLength, collisionCount, rate, move(collisionFrequencies)});
    }

    return summaries;
}

AvalancheSummary perform_avalanche_benchmark(HashAlgorithm algorithm, const BenchmarkConfig &config)
{
    size_t pairCount = max<size_t>(1, config.avalanchePairs);

    size_t hexMin = numeric_limits<size_t>::max();
    size_t hexMax = 0;
    double hexSum = 0.0;

    size_t bitMin = numeric_limits<size_t>::max();
    size_t bitMax = 0;
    double bitSum = 0.0;

    for (size_t i = 0; i < pairCount; ++i)
    {
    auto [first, second] = single_char_difference_pair(config.avalancheInputLength);
        const string hashA = run_hash(algorithm, first);
        const string hashB = run_hash(algorithm, second);

        size_t hexDiff = 0;
        size_t bitDiff = 0;

        for (size_t idx = 0; idx < hashA.size() && idx < hashB.size(); ++idx)
        {
            if (hashA[idx] != hashB[idx])
            {
                ++hexDiff;
            }
            const int nibbleA = hex_char_to_int(hashA[idx]);
            const int nibbleB = hex_char_to_int(hashB[idx]);
            bitDiff += popcount4(nibbleA ^ nibbleB);
        }

        hexMin = min(hexMin, hexDiff);
        hexMax = max(hexMax, hexDiff);
        hexSum += static_cast<double>(hexDiff);

        bitMin = min(bitMin, bitDiff);
        bitMax = max(bitMax, bitDiff);
        bitSum += static_cast<double>(bitDiff);
    }

    if (hexMin == numeric_limits<size_t>::max())
    {
        hexMin = 0;
    }
    if (bitMin == numeric_limits<size_t>::max())
    {
        bitMin = 0;
    }

    return {
        config.avalancheInputLength,
        pairCount,
        hexMin,
        hexMax,
        hexSum / static_cast<double>(pairCount),
        bitMin,
        bitMax,
        bitSum / static_cast<double>(pairCount)};
}

void write_benchmark_results_file(const fs::path &filename,
                                  HashAlgorithm algorithm,
                                  const BenchmarkConfig &config,
                                  const vector<TimingResult> &timings,
                                  const vector<CollisionSummary> &collisions,
                                  const AvalancheSummary &avalanche)
{
    ofstream out(filename, ios::app);
    if (!out.is_open())
    {
        throw runtime_error("Failed to open benchmark output file: " + filename.string());
    }

    out << string(60, '=') << '\n';
    out << "Benchmark report for " << hash_algorithm_name(algorithm) << '\n';

    const auto now = system_clock::now();
    const auto nowTime = system_clock::to_time_t(now);
    std::tm timeBuffer;
#if defined(_WIN32)
    localtime_s(&timeBuffer, &nowTime);
#else
    localtime_r(&nowTime, &timeBuffer);
#endif
    out << "Generated: " << put_time(&timeBuffer, "%Y-%m-%d %H:%M:%S") << "\n\n";

    out << "[1] konstitucija.txt throughput" << '\n';
    out << "    Repetitions per data point: " << config.konstitucijaRuns << "\n";
    out << "    Lines read | Avg time (ms)" << '\n';
    for (const auto &timing : timings)
    {
        out << "      " << setw(8) << timing.linesRead << " | " << fixed << setprecision(4)
            << timing.averageMilliseconds << '\n';
    }

    out << '\n' << "[2] Collision sampling" << '\n';
    out << "    Pairs per input length: " << config.collisionPairsPerLength << "\n";
    if (config.collisionInputLengths.size() > 1)
    {
        out << "    Input lengths sampled: ";
        for (size_t i = 0; i < config.collisionInputLengths.size(); ++i)
        {
            out << config.collisionInputLengths[i];
            if (i + 1 != config.collisionInputLengths.size())
                out << ", ";
        }
        out << '\n';
    }
    out << "    Length | Collisions | Rate" << '\n';
    for (const auto &summary : collisions)
    {
        out << "      " << setw(6) << summary.inputLength << " | " << setw(10)
            << summary.collisionPairs << " | " << fixed << setprecision(6)
            << (summary.totalPairs > 0 ? summary.collisionRate : 0.0) << '\n';
        if (!summary.collisionFrequencies.empty())
        {
            out << "        Hashes involved:" << '\n';
            for (const auto &entry : summary.collisionFrequencies)
            {
                out << "          " << entry.first << " -> " << entry.second << " occurrence(s)" << '\n';
            }
        }
    }
    if (collisions.empty())
    {
        out << "      (Collision sampling skipped)" << '\n';
    }

    out << '\n' << "[3] Avalanche effect" << '\n';
    out << "    Input length per pair: " << avalanche.inputLength << " characters" << '\n';
    out << "    Sampled pairs: " << avalanche.pairsEvaluated << '\n';
    out << "    Hex difference (min / avg / max): " << avalanche.hexMin << " / "
        << fixed << setprecision(4) << avalanche.hexAverage << " / " << avalanche.hexMax << '\n';
    out << "    Bit difference (min / avg / max): " << avalanche.bitMin << " / "
        << fixed << setprecision(4) << avalanche.bitAverage << " / " << avalanche.bitMax << '\n';

    out << '\n';
}