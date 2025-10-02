#include "tests.hpp"

#include <iomanip>
#include <limits>
#include <string>

using namespace std;

namespace
{
    size_t prompt_positive_size_t(const string &question, size_t defaultValue)
    {
        cout << question << " [default: " << defaultValue << "]: ";
        string line;
        getline(cin, line);
        if (line.empty())
        {
            return defaultValue;
        }

        try
        {
            size_t value = static_cast<size_t>(stoull(line));
            if (value == 0)
            {
                cout << "Value must be positive. Using default." << endl;
                return defaultValue;
            }
            return value;
        }
        catch (...)
        {
            cout << "Unrecognized input. Using default value." << endl;
            return defaultValue;
        }
    }

    HashAlgorithm prompt_algorithm()
    {
        cout << "Select algorithm (1 - PHA256, 2 - PHA256_AI, 3 - NojusHash, 4 - SHA256, 5 - KituHash): ";
        char algorithmChoice;
        cin >> algorithmChoice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return resolve_algorithm_choice(algorithmChoice);
    }

    void handle_file_creation()
    {
        cout << "Enter output file name: ";
        string filename;
        getline(cin, filename);

        cout << "Choose generation mode:\n"
             << "1 - Single random character\n"
             << "2 - 2000 random characters\n"
             << "3 - Pair of 2000 character strings differing in one character\n"
             << "4 - Empty file\n";

        char mode;
        cin >> mode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (filename.find(".txt") == string::npos)
        {
            filename += ".txt";
        }

        ofstream out(filename);
        if (!out.is_open())
        {
            cerr << "Failed to create file: " << filename << endl;
            return;
        }

        file_creation_tool(mode, out);
        cout << "File generated at: " << filename << endl;
    }

    void handle_output_length()
    {
        HashAlgorithm algorithm = prompt_algorithm();
        cout << "Enter input string: ";
        string input;
        getline(cin, input);

        string hash = run_hash(algorithm, input);
        cout << "Hash: " << hash << "\nLength: " << hash.size()
             << " symbols (expected " << expected_hash_length(algorithm) << ")" << endl;
    }

    void handle_determinism()
    {
        HashAlgorithm algorithm = prompt_algorithm();
        cout << "Enter input string: ";
        string input;
        getline(cin, input);

        string first = run_hash(algorithm, input);
        string second = run_hash(algorithm, input);

        cout << "First hash : " << first << '\n'
             << "Second hash: " << second << '\n'
             << (first == second ? "Determinism OK" : "Determinism FAILED") << endl;
    }

    void handle_konstitucija_benchmark()
    {
        HashAlgorithm algorithm = prompt_algorithm();
        cout << "How many repetitions per data point? [default: 1]: ";
        string line;
        getline(cin, line);
        int repetitions = 1;
        if (!line.empty())
        {
            try
            {
                repetitions = max(1, stoi(line));
            }
            catch (...)
            {
                cout << "Invalid value. Using 1." << endl;
                repetitions = 1;
            }
        }

        vector<TimingResult> results = perform_konstitucija_benchmark(algorithm, repetitions);
        for (const auto &entry : results)
        {
            cout << entry.linesRead << " lines -> " << fixed << setprecision(4)
                 << entry.averageMilliseconds << " ms" << endl;
        }
    }

    void handle_collision_benchmark()
    {
        HashAlgorithm algorithm = prompt_algorithm();
        BenchmarkConfig config;
        config.collisionPairsPerLength = prompt_positive_size_t("Pairs per input length", config.collisionPairsPerLength);
        config.konstitucijaRuns = 1;
        vector<CollisionSummary> summaries = perform_collision_benchmark(algorithm, config);
        for (const auto &summary : summaries)
        {
            cout << "Length " << summary.inputLength << ": " << summary.collisionPairs
                 << " collision(s) out of " << summary.totalPairs << " pairs (rate=" << fixed << setprecision(6)
                 << summary.collisionRate << ")" << endl;
            if (!summary.collisionFrequencies.empty())
            {
                cout << "Colliding hashes:" << endl;
                for (const auto &entry : summary.collisionFrequencies)
                {
                    cout << "  " << entry.first << " -> " << entry.second << endl;
                }
            }
        }
    }

    void handle_avalanche_benchmark()
    {
        HashAlgorithm algorithm = prompt_algorithm();
        BenchmarkConfig config;
        config.avalanchePairs = prompt_positive_size_t("How many pairs to sample?", config.avalanchePairs);
        config.avalancheInputLength = prompt_positive_size_t("Characters per string?", config.avalancheInputLength);
        AvalancheSummary summary = perform_avalanche_benchmark(algorithm, config);
        cout << "Hex diff (min/avg/max): " << summary.hexMin << '/' << fixed << setprecision(4)
             << summary.hexAverage << '/' << summary.hexMax << endl;
        cout << "Bit diff (min/avg/max): " << summary.bitMin << '/' << fixed << setprecision(4)
             << summary.bitAverage << '/' << summary.bitMax << endl;
        cout << "Pairs evaluated: " << summary.pairsEvaluated << ", characters per string: " << summary.inputLength << endl;
    }
}

int main()
{
    cout << "Welcome to the manual testing suite!" << endl;

    bool running = true;
    while (running)
    {
        cout << "\n--- Menu ---\n"
             << "1 - File creation utility\n"
             << "2 - Output length checker\n"
             << "3 - Determinism checker\n"
             << "4 - konstitucija.txt benchmark\n"
             << "5 - Collision benchmark\n"
             << "6 - Avalanche benchmark\n"
             << "0 - Exit\n"
             << "Select option: ";

        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice)
        {
        case '1':
            handle_file_creation();
            break;
        case '2':
            handle_output_length();
            break;
        case '3':
            handle_determinism();
            break;
        case '4':
            handle_konstitucija_benchmark();
            break;
        case '5':
            handle_collision_benchmark();
            break;
        case '6':
            handle_avalanche_benchmark();
            break;
        case '0':
            running = false;
            break;
        default:
            cout << "Unknown option. Please try again." << endl;
            break;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
