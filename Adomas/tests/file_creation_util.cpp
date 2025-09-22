#include "PHA.hpp"

int main()
{
    srand(time(NULL));

    cout << "Welcome to the File Creation Util!" << endl
         << "-----------------" << endl
         << "Name of file to create:" << endl;

    string outputFile;
    cin >> outputFile;
    ofstream outstream(outputFile + ".txt");

    cout << endl
         << "Type of file:" << endl
         << "1. 1 random character" << endl
         << "2. 2000 random characters" << endl
         << "3. Pair of 2000 random char files with 1 char different" << endl
         << "4. Empty file" << endl;

    char choice;
    cin >> choice;

    switch (choice)
    {
    case '1':
    {
        outstream << char('a' + rand() % 26);
        break;
    }

    case '2':
    {
        string s;

        for (int i = 0; i < 2000; i++)
        {
            s += 'a' + rand() % 26;
        }

        outstream << s;
        break;
    }

    case '3':
    {
        string s;
        ofstream outstream2(outputFile + "_2.txt");

        for (int i = 0; i < 2000; i++)
        {
            s += 'a' + rand() % 26;
        }

        outstream << s << char('a' + rand() % 26);
        outstream2 << s << char('a' + rand() % 26);
        break;
    }

    case '4':
        break;

    default:
        break;
    }
}
