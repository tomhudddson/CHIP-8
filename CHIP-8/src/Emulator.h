#pragma once

#include "Chip8.h"
#include "Display.h"

class Emulator
{
public:
    Emulator();

    void Run();

private:
    Chip8 c8;
    Display display;
};