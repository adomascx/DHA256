#include "tests.hpp"

#include <cctype>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;
namespace fs = std::filesystem;

namespace
{
	vector<HashAlgorithm> prompt_algorithms()
	{
		cout << "Choose algorithm(s):\n"
		     << "1 - PHA256\n"
		     << "2 - PHA256_AI\n"
		     << "3 - NojusHash\n"
		     << "4 - SHA256\n"
		     << "5 - KituHash\n"
		     << "6 - All" << endl;
		cout << "Selection: ";
		char choice;
		cin >> choice;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');

		switch (choice)
		{
		case '1':
			return {HashAlgorithm::PHA256_Basic};
		case '2':
			return {HashAlgorithm::PHA256_AI};
		case '3':
			return {HashAlgorithm::Nojus};
		case '4':
			return {HashAlgorithm::SHA256};
		case '5':
			return {HashAlgorithm::KituHash};
		case '6':
			return {HashAlgorithm::PHA256_Basic, HashAlgorithm::PHA256_AI, HashAlgorithm::Nojus, HashAlgorithm::SHA256, HashAlgorithm::KituHash};
		default:
			cout << "Unrecognized option. Defaulting to PHA256." << endl;
			return {HashAlgorithm::PHA256_Basic};
		}
	}

	fs::path benchmark_output_path()
	{
		fs::path cwd = fs::current_path();
		if (cwd.filename() == "build")
		{
			return cwd.parent_path() / "benchmark_results.txt";
		}
		return cwd / "benchmark_results.txt";
	}

	size_t prompt_size_t(const string &question, size_t defaultValue)
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

	bool run_tests_mode(const vector<HashAlgorithm> &algorithms)
	{
		const vector<string> inputs = default_test_inputs();
		bool allPassed = true;

		for (HashAlgorithm algorithm : algorithms)
		{
			cout << "\nTesting " << hash_algorithm_name(algorithm) << "..." << endl;

			bool lengthOk = validate_output_length(algorithm, inputs);
			bool determinismOk = validate_determinism(algorithm, inputs);

			cout << "  Output length (" << expected_hash_length(algorithm) << " chars): "
			     << (lengthOk ? "PASS" : "FAIL") << '\n'
				 << "  Determinism: " << (determinismOk ? "PASS" : "FAIL") << endl;

			allPassed = allPassed && lengthOk && determinismOk;
		}

		return allPassed;
	}

	void run_benchmark_mode(const vector<HashAlgorithm> &algorithms)
	{
		cout << "How many runs for konstitucija benchmark? ";
		int repetitions;
		cin >> repetitions;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if (repetitions <= 0)
		{
			repetitions = 1;
		}

		BenchmarkConfig config;
		config.konstitucijaRuns = repetitions;
		config.collisionPairsPerLength = prompt_size_t("Random pairs per input length for collision sampling", config.collisionPairsPerLength);
		config.avalanchePairs = prompt_size_t("Pairs to sample for avalanche benchmark", config.avalanchePairs);
		config.avalancheInputLength = prompt_size_t("Characters per string for avalanche benchmark", config.avalancheInputLength);

		const fs::path outputFile = benchmark_output_path();

		// Clear previous content to keep reports fresh per benchmark session.
		ofstream clearFile(outputFile, ios::trunc);
		clearFile.close();

		for (HashAlgorithm algorithm : algorithms)
		{
			cout << "\nRunning benchmarks for " << hash_algorithm_name(algorithm) << "..." << endl;

			vector<TimingResult> timings = perform_konstitucija_benchmark(algorithm, config.konstitucijaRuns);
			vector<CollisionSummary> collisions = perform_collision_benchmark(algorithm, config);
			AvalancheSummary avalanche = perform_avalanche_benchmark(algorithm, config);

			write_benchmark_results_file(outputFile, algorithm, config, timings, collisions, avalanche);

			cout << "Konstitucija.txt throughput (" << config.konstitucijaRuns << " run(s) per data point):" << endl;
			cout << "  Lines read | Avg ms" << endl;
			cout << fixed << setprecision(4);
			for (const auto &timing : timings)
			{
				cout << "  " << setw(9) << timing.linesRead << " | " << timing.averageMilliseconds << endl;
			}
			cout.unsetf(ios::floatfield);

			cout << "Collision sampling (pairs per length: " << config.collisionPairsPerLength << "):" << endl;
			if (collisions.empty())
			{
				cout << "  (No collision sampling performed)" << endl;
			}
			else
			{
				cout << fixed << setprecision(6);
				for (const auto &summary : collisions)
				{
					cout << "  len=" << setw(5) << summary.inputLength << ": " << summary.collisionPairs
						 << " collision(s), rate=" << summary.collisionRate << endl;
					if (!summary.collisionFrequencies.empty())
					{
						cout << "    hashes:" << endl;
						for (const auto &entry : summary.collisionFrequencies)
						{
							cout << "      " << entry.first << " -> " << entry.second << endl;
						}
					}
				}
				cout.unsetf(ios::floatfield);
			}

			cout << "Avalanche sampling (" << avalanche.pairsEvaluated << " pair(s), "
				 << avalanche.inputLength << " characters per string):" << endl;
			cout << fixed << setprecision(4)
				 << "  Hex difference min/avg/max: " << avalanche.hexMin << " / " << avalanche.hexAverage
				 << " / " << avalanche.hexMax << endl
				 << "  Bit difference min/avg/max: " << avalanche.bitMin << " / " << avalanche.bitAverage
				 << " / " << avalanche.bitMax << endl;
			cout.unsetf(ios::floatfield);
			cout << "Results appended to: " << outputFile << "\n";
		}
	}
}

int main()
{
	cout << "Run benchmark or tests? (b/t): ";
	char mode;
	cin >> mode;
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	vector<HashAlgorithm> algorithms = prompt_algorithms();

	mode = static_cast<char>(tolower(mode));
	if (mode == 't')
	{
		if (!run_tests_mode(algorithms))
		{
			cerr << "One or more tests failed." << endl;
			return 1;
		}
	}
	else
	{
		run_benchmark_mode(algorithms);
	}

	cout << "Done." << endl;
	return 0;
}