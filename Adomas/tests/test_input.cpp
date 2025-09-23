#include "PHA.hpp"

int main(int argc, const char *argv[])
{
	string inputString;
	if (argc < 2)
	{
		cin >> inputString;
		cout << "Hash: " << PHA256(inputString);
	}
	else
	{
		ifstream inputStream(string(argv[1]) + ".txt");
		getline(inputStream, inputString);
		cout << "Hash: " << PHA256(inputString);
	}
}
