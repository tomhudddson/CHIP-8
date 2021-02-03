#include <iostream>

#include "Chip8.h"

int main()
{
    Chip8 c8;


    while (1)
    {
        c8.Cycle();
    }
    std::cout << "fuck" << std::endl;
}