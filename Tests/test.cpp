#include "pch.h"

#include "../CHIP-8/src/Chip8.h"
#include "../CHIP-8/src/Chip8.cpp"

TEST(OPTest, CLSTest)
{
    Chip8 c;

    // Fill display with rubbish.
    for (size_t i = 0; i < C8_DISPLAY_SZ; i++)
    {
        c.display[i] = 4;
    }

    // Now clear the display and check if all values are 0.
    c.OP_00E0();

    bool zero = true;
    for (size_t i = 0; i < C8_DISPLAY_SZ; i++)
    {
        if (c.display[i] != 0)
        {
            zero = false;
            break;
        }
    }

    ASSERT_EQ(zero, true);
}