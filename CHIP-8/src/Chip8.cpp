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
    std::ifstream file("test_roms/IBM Logo.ch8", std::ios::binary | std::ios::ate);

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
    ((*this).*(optable[(opcode & 0xF000u) >> 12u]))();

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
    std::cout << "OP_00E0 called" << std::endl;
    memset(display, 0, sizeof(display));
}

/* This function returns from a subroutine by setting the program counter to 
the top of the stack, then decrementing the stack pointer. */
void Chip8::OP_00EE()
{
}

/* This function jumps to an address, nnn, by setting the program counter to 
nnn. */
void Chip8::OP_1nnn()
{
    std::cout << "OP_1nnn called" << std::endl;
    pc = opcode & 0x0FFFu;
}

/* This function calls a subroutine by setting the current program counter to 
the top of the stack, incrementing the stack pointer, then setting the opcode
to nnn. */
void Chip8::OP_2nnn()
{
    std::cout << "OP_2nnn called" << std::endl;
    stack[sp++] = pc;
    pc = opcode & 0x0FFFu;
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

/* This function stores the value of kk into the register Vx. */
void Chip8::OP_6xkk()
{
    std::cout << "OP_6xkk called" << std::endl;
    regs[(opcode & 0x0F00u) >> 8] = opcode & 0xFFu;
}

/* This function stores the value of Vx + kk ino the register Vx. */
void Chip8::OP_7xkk()
{
    std::cout << "OP_7xkk called" << std::endl;
    uint8_t regindex = (opcode & 0x0F00u) >> 8;
    regs[regindex] = regs[regindex] + (opcode & 0x00FFu);
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

/* This function stores the value nnn into the register I. */
void Chip8::OP_Annn()
{
    std::cout << "OP_00E0 called" << std::endl;
    I = opcode & 0x0FFFu;
}

void Chip8::OP_Bnnn()
{
}

void Chip8::OP_Cxkk()
{
}

/* This is the draw function. */
void Chip8::OP_Dxyn()
{
    std::cout << "OP_Dxyn called" << std::endl;
    // Get x and y coordinates where the sprite will be drawn from Vx and Vy.
    uint16_t x = regs[(opcode & 0x0F00u) >> 8];
    uint16_t y = regs[(opcode & 0x00F0u) >> 4];
    uint16_t height = opcode & 0x000Fu;

    // Modulo coordinates to ensure they wrap.
    x %= C8_DISPLAY_WIDTH;
    y %= C8_DISPLAY_HEIGHT;

    // Clear VF.
    regs[0xF] = 0;

    for (uint_fast16_t row = 0; row < height; row++)
    {
        uint8_t sprdat = mem[I + row];
        
        for (uint_fast8_t col = 0; col < 8; col++)
        {
            // Shift a 1 through the word starting at MSB and finishing at LSB
            // to target each bit once during this loop. 
            uint8_t sprByte = sprdat & (0x80 >> col);  

            // Find the pixel in the display of the sprite pixel.
            // uint32_t* displayByte = &display[(row + y) * C8_DISPLAY_HEIGHT 
            //                              + (col * y)];
            uint32_t* displayByte = &display[x + col + (y + row) * 64];

            if (displayByte)
            {
                if (*displayByte == 0xFFFFFFFF)
                {
                    // This pixel is already on, so there is a collision.
                    regs[0xF] = 1;
                }

                // Since the sprite pixel is on here, we can simply XOR the 
                // display pixel 0xFFFFFFFF to toggle it.
                *displayByte ^= 0xFFFFFFFF;
            } 
        }
    }
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
    std::cout << "OP_Null called" << std::endl;
}
