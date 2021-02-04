#include "Emulator.h"

#include <iostream>



Emulator::Emulator()
{
    if (!display.Init("Chip 8 Emulator", 20))
    {
        std::cout << "Could not create display!" << std::endl;
    }
}

void Emulator::Run()
{
    while (display.IsRunning())
    {
        display.HandleEvents();

        c8.Cycle();
        display.Update();

        display.Render(c8.GetDisplay());
    }
}