#include "PHA.hpp"

int main(int argc, const char *argv[])
{
	cout << "Choose your algorithm:\n1 - PHA256\n2 - PHA256_AI\n\n";
	char choice;
	cin >> choice;

	string inputString;
	if (argc < 2)
	{
		cout << "Input: ";
		cin >> inputString;
		cout << "Hash: " << (choice == '1' ? PHA256(inputString) : PHA256_AI(inputString));
	}
	else
	{
		string filename = argv[1];
		if (filename.find(".txt") == string::npos)
		{
			filename += ".txt";
		}
		ifstream inputStream(filename);
		getline(inputStream, inputString);
		cout << "Hash: " << (choice == '1' ? PHA256(inputString) : PHA256_AI(inputString));
	}
}
