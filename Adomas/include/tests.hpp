#pragma once

#include "PHA256.hpp"

#include <vector>
#include <unordered_map>
#include <filesystem>

enum class HashAlgorithm
{
	PHA256_Basic = 1,
	PHA256_AI = 2,
	Nojus = 3,
	SHA256 = 4,
	KituHash = 5
};

struct TimingResult
{
	size_t linesRead;
	double averageMilliseconds;
};

struct CollisionSummary
{
	size_t inputLength;
	size_t totalPairs;
	size_t collisionPairs;
	double collisionRate;
	unordered_map<string, size_t> collisionFrequencies;
};

struct AvalancheSummary
{
	size_t inputLength;
	size_t pairsEvaluated;
	size_t hexMin;
	size_t hexMax;
	double hexAverage;
	size_t bitMin;
	size_t bitMax;
	double bitAverage;
};

struct BenchmarkConfig
{
	int konstitucijaRuns = 3;
	size_t collisionPairsPerLength = 2000;
	std::vector<size_t> collisionInputLengths = {10, 100, 500, 1000};
	size_t avalanchePairs = 2000;
	size_t avalancheInputLength = 1024;
};

void file_creation_tool(int toolNum, ostream &outStream);

HashAlgorithm resolve_algorithm_choice(char choice);
string hash_algorithm_name(HashAlgorithm algorithm);
string run_hash(HashAlgorithm algorithm, const string &input);
size_t expected_hash_length(HashAlgorithm algorithm);
string sha256_hex(const string &input);

bool validate_output_length(HashAlgorithm algorithm, const vector<string> &inputs);
bool validate_determinism(HashAlgorithm algorithm, const vector<string> &inputs);

vector<string> default_test_inputs();

vector<TimingResult> perform_konstitucija_benchmark(HashAlgorithm algorithm, int repetitions);
vector<CollisionSummary> perform_collision_benchmark(HashAlgorithm algorithm, const BenchmarkConfig &config);
AvalancheSummary perform_avalanche_benchmark(HashAlgorithm algorithm, const BenchmarkConfig &config);

void write_benchmark_results_file(const filesystem::path &filename,
								  HashAlgorithm algorithm,
								  const BenchmarkConfig &config,
								  const vector<TimingResult> &timings,
								  const vector<CollisionSummary> &collisions,
								  const AvalancheSummary &avalanche);