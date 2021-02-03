#include "Chip8.h"

#include <memory>
#include <fstream>
#include <iostream>

Chip8::Chip8()
{
    LoadCharacterSet();
    LoadROM();
    pc = C8_START_ADDR;
}

/* This function loads ROM stored in a text file into memory. */
void Chip8::LoadROM()
{
    // Open the file as a stream of binary and move the file pointer to the end
    std::ifstream file("test_roms/test.ch8", std::ios::binary | std::ios::ate);

    if (file.is_open())
    {
        // Get size of file and allocate a buffer to hold the contents.
        std::streampos size = file.tellg();
        char* buffer = new char[size];

        // Go back to the beginning of the file and fill the buffer.
        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        // Load the ROM contents into the Chip8's memory, starting at 0x200.
        for (long i = 0; i < size; ++i)
        {
            mem[C8_START_ADDR + i] = buffer[i];
        }

        // Free the buffer
        delete[] buffer;
    }
}

/* This function loads the character set into memory. */
void Chip8::LoadCharacterSet()
{
    for (size_t i = 0; i < C8_CHARSET_SZ; i++)
    {
        mem[C8_CHARSET_START_ADDR + i] = charset[i];
    }
}

void Chip8::Cycle()
{
    // Fetch next instruction from memory.
    opcode = (mem[pc] << 8) | mem[pc + 1];

    // Increment program counter by 4 bytes so it is at the next address 
    // before executing the next instruction.
    pc += 2;

    // Execute instruction. For now this uses a switch statement, eventually
    // it will use function pointers and arrays.
    ExecuteOpcode();

    // Decrement delay timer and sound timer if they are set.
    if (delayTimer > 0)
        --delayTimer;

    if (soundTimer > 0)
        --soundTimer;
}

void Chip8::ExecuteOpcode()
{
    switch (opcode)
    {
    case C8_OP_00E0:
        OP_00E0();
        break;
    case C8_OP_00EE:
        OP_00EE();
        break;
    case C8_OP_2nnn:
        OP_2nnn();
        break;
    default:
        std::cout << "Did not recongnise that opcode!" << std::endl;
    }
}

/* This function clears the display by setting all values in the display array
to 0. */
void Chip8::OP_00E0()
{   
    std::cout << "OP_00EO executed" << std::endl;
    memset(display, 0, sizeof(display));
}

void Chip8::OP_00EE()
{
    std::cout << "OP_00EE executed" << std::endl;

}

void Chip8::OP_1nnn()
{
}

/* This function calls a subroutine by incrementing the stack pointer, putting 
the current PC on top of the the stack, and setting the PC to nnn. */
void Chip8::OP_2nnn()
{
    std::cout << "OP_1nnn executed" << std::endl;
    stack[sp++] = pc;
    pc = opcode & 0xFFFu;
}

void Chip8::OP_3xkk()
{
}

void Chip8::OP_4xkk()
{
}

void Chip8::OP_5xy0()
{
}

void Chip8::OP_6xkk()
{
}

void Chip8::OP_7xkk()
{
}

void Chip8::OP_8xy0()
{
}

void Chip8::OP_8xy1()
{
}

void Chip8::OP_8xy2()
{
}

void Chip8::OP_8xy3()
{
}

void Chip8::OP_8xy4()
{
}

void Chip8::OP_8xy5()
{
}

void Chip8::OP_8xy6()
{
}

void Chip8::OP_8xy7()
{
}

void Chip8::OP_8xyE()
{
}

void Chip8::OP_9xy0()
{
}

void Chip8::OP_Annn()
{
}

void Chip8::OP_Bnnn()
{
}

void Chip8::OP_Cxkk()
{
}

void Chip8::OP_Dxyn()
{
}

void Chip8::OP_Ex9E()
{
}

void Chip8::OP_ExA1()
{
}

void Chip8::OP_Fx07()
{
}

void Chip8::OP_Fx0A()
{
}

void Chip8::OP_Fx15()
{
}

void Chip8::OP_Fx18()
{
}

void Chip8::OP_Fx1E()
{
}

void Chip8::OP_Fx29()
{
}

void Chip8::OP_Fx33()
{
}

void Chip8::OP_Fx55()
{
}

void Chip8::OP_Fx65()
{
}

