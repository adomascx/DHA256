#include "PHA.hpp"

int main()
{
    srand(time(NULL));

    cout << "Welcome to the File Creation Util!" << endl
         << "-----------------" << endl
         << "Name of file to create:" << endl;

    string outputFile;
    cin >> outputFile;
    ofstream outStream(outputFile + ".txt");

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
        outStream << char('a' + rand() % 26);
        break;
    }

    case '2':
    {
        string s;

        for (int i = 0; i < 2000; i++)
        {
            s += 'a' + rand() % 26;
        }

        outStream << s;
        break;
    }

    case '3':
    {
        string s;
        ofstream outStream2(outputFile + "_2.txt");

        for (int i = 0; i < 2000; i++)
        {
            s += 'a' + rand() % 26;
        }

        outStream << s << char('a' + rand() % 26);
        outStream2 << s << char('a' + rand() % 26);
        break;
    }

    case '4':
        break;

    default:
        break;
    }
}
