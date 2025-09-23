#include "header.h"

int main()
{
    std::cout << "Skaityti faila - 1, Ivesti string - 2: ";
    std::cout << std::endl;
    int choice = getch();
    while(choice != '1' && choice != '2'){
        std::cout << "Neteisinga ivestis. Skaityti faila - 1, Ivesti string - 2: ";
        std::cout << std::endl;
        choice = getch();
    }
    if (choice == '1'){
        Skaityti();
    }
    else if(choice == '2'){
        std::cout << "Iveskite string: ";
        Ivesti();
    }

    return 0;
}