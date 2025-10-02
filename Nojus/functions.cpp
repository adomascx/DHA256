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
        value += (value >> 2) | (value << 30);
    }
    uint32_t part1 = value;
    uint32_t part2 = ~value;
    uint32_t part3 = value ^ 0xFFFF;
    uint32_t part4 = value * 3;
    uint32_t part5 = (value << 1) ^ (value >> 1);
    uint32_t part6 = value + 0xA5A5A5A5;
    uint32_t part7 = value ^ 0x7E7E7E7E;
    uint32_t part8 = (value ^ 0xFFFFFFFF) * (value ^ 0x12345678);

    std::stringstream hash;
    hash << std::hex
         << std::setw(8) << std::setfill('0') << part1
         << std::setw(8) << std::setfill('0') << part2
         << std::setw(8) << std::setfill('0') << part3
         << std::setw(8) << std::setfill('0') << part4
         << std::setw(8) << std::setfill('0') << part5
         << std::setw(8) << std::setfill('0') << part6
         << std::setw(8) << std::setfill('0') << part7
         << std::setw(8) << std::setfill('0') << part8;

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
    std::cout << std::endl
              << "Total words found: " << stringArray.size() << std::endl;

    std::vector<std::string> hashVector;
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
            value += (value >> 2) | (value << 30);
        }
        uint32_t part1 = value;
        uint32_t part2 = ~value;
        uint32_t part3 = value ^ 0xFFFF;
        uint32_t part4 = value * 3;
        uint32_t part5 = (value << 1) ^ (value >> 1);
        uint32_t part6 = value + 0xA5A5A5A5;
        uint32_t part7 = value ^ 0x7E7E7E7E;
        uint32_t part8 = (value ^ 0xFFFFFFFF) * (value ^ 0x12345678);

        std::stringstream hash;
        hash << std::hex
             << std::setw(8) << std::setfill('0') << part1
             << std::setw(8) << std::setfill('0') << part2
             << std::setw(8) << std::setfill('0') << part3
             << std::setw(8) << std::setfill('0') << part4
             << std::setw(8) << std::setfill('0') << part5
             << std::setw(8) << std::setfill('0') << part6
             << std::setw(8) << std::setfill('0') << part7
             << std::setw(8) << std::setfill('0') << part8;

        std::string hashStr = hash.str();
        hashVector.push_back(hashStr);
        std::ofstream fr("hash.txt", std::ios::app);
        fr << "Word: '" << str << "' -> Hash: " << hashStr << std::endl;
        fr.close();
    }

    std::cout << "Checking for hash collisions..." << std::endl;
    bool foundCollision = false;
    for (size_t i = 0; i < hashVector.size(); i++)
    {
        for (size_t j = i + 1; j < hashVector.size(); j++)
        {
            if (hashVector[i] == hashVector[j] && stringArray[i] != stringArray[j])
            {
                std::cout << "COLLISION FOUND! Hash " << hashVector[i] << " appears for words '" << stringArray[i] << "' and '" << stringArray[j] << "'" << std::endl;
                foundCollision = true;
            }
        }
    }
    if (!foundCollision)
    {
        std::cout << "No hash collisions found among " << hashVector.size() << " hashes." << std::endl;
    }
}