#include "Chip8.h"

#include <memory>
#include <fstream>
#include <iostream>

Chip8::Chip8()
{
    LoadCharacterSet();
    LoadROM();
    LoadFunctionPointerTable();
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

void Chip8::LoadFunctionPointerTable()
{
    optable[0x0] = &Chip8::Table0;  
    optable[0x1] = &Chip8::OP_1nnn;
    optable[0x2] = &Chip8::OP_2nnn;
    optable[0x3] = &Chip8::OP_3xkk;
    optable[0x4] = &Chip8::OP_4xkk;
    optable[0x5] = &Chip8::OP_5xy0;
    optable[0x6] = &Chip8::OP_6xkk;
    optable[0x7] = &Chip8::OP_7xkk;
    optable[0x8] = &Chip8::Table8;
    optable[0x9] = &Chip8::OP_9xy0;
    optable[0xA] = &Chip8::OP_Annn;
    optable[0xB] = &Chip8::OP_Bnnn;
    optable[0xC] = &Chip8::OP_Cxkk;
    optable[0xD] = &Chip8::OP_Dxyn;
    optable[0xE] = &Chip8::TableE;
    optable[0xF] = &Chip8::TableF;

    optable0[0x0] = &Chip8::OP_00E0;
    optable0[0xE] = &Chip8::OP_00EE;

    optableE[0x1] = &Chip8::OP_ExA1;
    optableE[0xE] = &Chip8::OP_Ex9E;

    optableF[0x07] = &Chip8::OP_Fx07;
    optableF[0x0A] = &Chip8::OP_Fx0A;
    optableF[0x15] = &Chip8::OP_Fx15;
    optableF[0x1E] = &Chip8::OP_Fx1E;
    optableF[0x18] = &Chip8::OP_Fx18;
    optableF[0x29] = &Chip8::OP_Fx29;
    optableF[0x33] = &Chip8::OP_Fx33;
    optableF[0x55] = &Chip8::OP_Fx55;
    optableF[0x65] = &Chip8::OP_Fx65;
}

void Chip8::Table0()
{
    ((*this).*(optable0[opcode & 0x000Fu]))();
}

void Chip8::Table8()
{
    ((*this).*(optable8[opcode & 0x000Fu]))();
}

void Chip8::TableE()
{
    ((*this).*(optableE[opcode & 0x000Fu]))();
}

void Chip8::TableF()
{
    ((*this).*(optable0[opcode & 0x00FFu]))();
}

void Chip8::Cycle()
{
    // Fetch next instruction from memory.
    opcode = (mem[pc] << 8) | mem[pc + 1];

    // Increment program counter by 2 bytes so it is at the next address 
    // before executing the next instruction.
    pc += 2;

    // In optable, only the first digit of the opcode is required to identify
    // the opcode. Therefore decode the first figit and call the relevant 
    // function optable.
    uint16_t index = (opcode & 0xF000u) >> 12u;
    ((*this).*(optable[index]))();

    // Decrement delay timer and sound timer if they are set.
    if (delayTimer > 0)
        --delayTimer;

    if (soundTimer > 0)
        --soundTimer;
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

void Chip8::OP_Null()
{
    
}
