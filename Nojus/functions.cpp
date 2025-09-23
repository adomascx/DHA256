#include "header.h"

void Ivesti()
{
    std::string string;
    std::cin >> string;
    int seed = 2671109;
    for (unsigned char c : string)
    {
        seed += (int)(unsigned char)c;
    }
    srand(seed);
    uint32_t value = rand();
    for (size_t i = 0; i < string.length(); i++)
    {
        uint32_t c = (unsigned char)string[i];
        int randomShift = rand() % 8;
        uint32_t rotatedValue;
        if (rand() % 2 == 0)
            rotatedValue = (c << randomShift) | (c >> (8 - randomShift));
        else
            rotatedValue = (c >> randomShift) | (c << (8 - randomShift));
        value ^= rotatedValue;
        value = (value << 5) | (value >> 27);
        value += (value >> 2);
    }
    uint32_t part1 = value;
    uint32_t part2 = ~value;
    uint32_t part3 = value ^ 0xFFFF;
    uint32_t part4 = value * 3;

    std::stringstream hash;
    hash << std::hex
         << std::setw(8) << std::setfill('0') << part1
         << std::setw(8) << std::setfill('0') << part2
         << std::setw(8) << std::setfill('0') << part3
         << std::setw(8) << std::setfill('0') << part4;

    std::cout << "Hash: " << hash.str() << std::endl;
}

void Skaityti()
{
    std::cout << "Failo pavadinimas: ";
    std::string filename;
    std::cin >> filename;
    std::ifstream file(filename + ".txt");
    if (!file.is_open())
    {
        std::cerr << "Nepavyko atidaryti failo." << std::endl;
        exit(1);
    }
    std::vector<std::string> stringArray;
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string word;
        while (ss >> word)
        {
            stringArray.push_back(word);
        }
    }
    file.close();
    int seed = 2671109;
    for (const std::string &str : stringArray)
    {
        for (unsigned char c : str)
        {
            seed += (int)(unsigned char)c;
        }
    }
    srand(seed);
    for (const std::string &str : stringArray)
    {
        uint32_t value = rand();
        for (size_t i = 0; i < str.length(); i++)
        {
            uint32_t c = (unsigned char)str[i];
            int randomShift = rand() % 8;
            uint32_t rotatedValue;
            if (rand() % 2 == 0)
                rotatedValue = (c << randomShift) | (c >> (8 - randomShift));
            else
                rotatedValue = (c >> randomShift) | (c << (8 - randomShift));
            value ^= rotatedValue;
            value = (value << 5) | (value >> 27);
            value += (value >> 2);
        }
        uint32_t part1 = value;
        uint32_t part2 = ~value;
        uint32_t part3 = value ^ 0xFFFF;
        uint32_t part4 = value * 3;

        std::stringstream hash;
        hash << std::hex
             << std::setw(8) << std::setfill('0') << part1
             << std::setw(8) << std::setfill('0') << part2
             << std::setw(8) << std::setfill('0') << part3
             << std::setw(8) << std::setfill('0') << part4;

        std::cout << "Hash: " << hash.str() << std::endl;
    }
}