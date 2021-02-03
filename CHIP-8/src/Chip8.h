#pragma once

#include <cstdint>

#define C8_CHARSET_SZ         0x50
#define C8_CHARSET_START_ADDR 0x00  // char set start address in memory
#define C8_CHARSET_END_ADDR   0x4F  // 0x50 chars in set, so max is 1 byte less

#define C8_DISPLAY_SZ 64 * 32

#define C8_START_ADDR         0x200 // accessible memory starts here

/* Opcodes. */
#define C8_OP_00E0 0x00E0
#define C8_OP_00EE 0x00EE
#define C8_OP_1nnn 0x1nnn
#define C8_OP_2nnn 0x2nnn
#define C8_OP_3xkk 0x3xkk
#define C8_OP_4xkk 0x4xkk
#define C8_OP_5xy0 0x5xy0
#define C8_OP_6xkk 0x6xkk
#define C8_OP_7xkk 0x7xkk
#define C8_OP_8xy0 0x8xy0
#define C8_OP_8xy1 0x8xy1
#define C8_OP_8xy2 0x8xy2
#define C8_OP_8xy3 0x8xy3
#define C8_OP_8xy4 0x8xy4
#define C8_OP_8xy5 0x8xy5
#define C8_OP_8xy6 0x8xy6
#define C8_OP_8xy7 0x8xy7
#define C8_OP_8xyE 0x8xyE
#define C8_OP_9xy0 0x9xy0
#define C8_OP_Annn 0xAnnn
#define C8_OP_Bnnn 0xBnnn
#define C8_OP_Cxkk 0xCxkk
#define C8_OP_Dxyn 0xDxyn
#define C8_OP_Ex9E 0xEx9E
#define C8_OP_ExA1 0xExA1
#define C8_OP_Fx07 0xFx07
#define C8_OP_Fx0A 0xFx0A
#define C8_OP_Fx15 0xFx15
#define C8_OP_Fx18 0xFx18
#define C8_OP_Fx1E 0xFx1E
#define C8_OP_Fx29 0xFx29
#define C8_OP_Fx33 0xFx33
#define C8_OP_Fx55 0xFx55
#define C8_OP_Fx65 0xFx65

class Chip8
{
public:
    Chip8();

    void Cycle();

private:
    /* Chip-8 is capable of accessing up to 4096 bytes of RAM from 0x000 to 
    0xFFF. The first 512 bytes, 0x000 to 0x1FF, is where the original 
    interpreter is located so should not be used by programs. Programs can
    therefore start at 0x200.*/
    uint8_t mem[4096]{};

    /* Chip-8 contains 16 8-bit registers. These are refered to as Vx, where x
    is the register number in hex (0 to F). VF is used as a flag by some 
    instructions so should not be used by any programs. */
    uint8_t regs[16]{};

    /* Chip-8 contains a single 16-bit register to store memory addresses. */
    uint16_t I{};

    /* This is a special prupose register that stores the value of the delay
    timer. This value, when non-zero, gets decremented by 1 at a rate of 
    60 MHz until it is 0 again. */
    uint8_t delayTimer{};

    /* This is a special purpose register that stores the value of the sound 
    timer. This value, when non-zero, gets decremented by 1 at a rate of 60
    MHz until it is 0 again. When non-zero, the Chip-8s buzzer will sound.*/
    uint8_t soundTimer{};

    /* Program counter - used to store the currently executing address. */
    uint16_t pc{};

    /* Stack pointer - used to point to the top of the stack. */
    uint8_t sp{};

    /* Chip-8 stack can store 16 16-bit values. It is used to store the address
    that the interpreter should return to when it has finished a subroutine. 
    Therefore, there can be 16 levels of nested subroutines. */
    uint16_t stack[16]{};

    /* The keyboard on the original Chip-8 contained a 16 key layout. */
    uint8_t keyboard[16]{};

    /* The original display contained a 64x32-pixel layout on a monochrome
    display. */
    uint8_t display[C8_DISPLAY_SZ]{};

    /* All instructions are 2 bytes long and are stored MSB first. In memory,
    the first byte of each instruction is located at an even address. If a 
    program includes sprite data, it should be padded to ensure the next
    instruction sits at an even number. */
    uint16_t opcode{};

    /* Each character is 8x5 pixels, therefore one character can be stored in 
    5 bytes, with each byte storing 1 pixel. From the keyboard, there are 16 
    possible characters, so 80 bytes are required to store the character set. */
    uint8_t charset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    void LoadROM();
    void LoadCharacterSet();

    void ExecuteOpcode();

    
    void OP_00E0();  // CLS - clear the display
    void OP_00EE();  // RET - return from a subroutine
    void OP_1nnn();  // JP addr - jump to location nnn
    void OP_2nnn();  // CALL addr - call subroutine at location nnn
    void OP_3xkk();  // SE Vx, byte - skip next instruction if Vx == kk
    void OP_4xkk();  // SNE Vx, byte - skip next instruction if Vx != kk
    void OP_5xy0();  // SE Vx, Vy - skip next instruction if Vx == Vy
    void OP_6xkk();  // LD Vx, byte - set Vx = byte
    void OP_7xkk();  // ADD Vx, byte - set Vx = Vx + byte
    void OP_8xy0();  // LD Vx, Vy - set Vx = Vy
    void OP_8xy1();  // OR Vx, Vy - set Vx = Vx OR Vy
    void OP_8xy2();  // AND Vx, Vy - set Vx = Vx AND Vy
    void OP_8xy3();  // XOR Vx, Vy - set Vx = Vx XOR Vy
    void OP_8xy4();  // ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
    void OP_8xy5();  // SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
    void OP_8xy6();  // SHR Vx {, Vy} - set Vx = Vx SHR 1
    void OP_8xy7();  // SUBN Vx, Vy - set Vx - Vy - Vx, set VF = NOT borrow
    void OP_8xyE();  // SHL Vx, {, Vy} - set Vx = Vx SHL 1
    void OP_9xy0();  // SNE Vx, Vy - skip next instruction if Vx != Vy
    void OP_Annn();  // LD I, addr - set I = nnn
    void OP_Bnnn();  // JP V0, addr - jump to location nnn + V0
    void OP_Cxkk();  // RND Vx, byte - set Vx = random byte AND kk
    void OP_Dxyn();  // DRW Vx, Vy, nibble - display n-byte sprite starting at      
                     //   memory locationI at (Vx, Vy), set VF = collision
    void OP_Ex9E();  // SKP Vx - skip next instruction if key with value Vx is
                     //   pressed
    void OP_ExA1();  // SKNP Vx - skip next instruction if the key with the 
                     //   value Vx is not pressed.
    void OP_Fx07();  // LD Vx, DT - Set Vx = delay timer value
    void OP_Fx0A();  // LD Vx, K - wait for a key press and store the value in 
                     //   Vx
    void OP_Fx15();  // LD DT, Vx - set delay timer = Vx
    void OP_Fx18();  // LD ST, Vx - set sound tomer = Vx
    void OP_Fx1E();  // ADD I, Vx - set I = I + Vx
    void OP_Fx29();  // LD F, Vx - set I = location of sprite for digit Vx
    void OP_Fx33();  // LD B, Vx - store BCD representation in memory locations 
                     //   I, I + 1, and I + 2
    void OP_Fx55();  // LD [I], Vx - store registers V0 through Vx in memory
                     //   starting at location I.
    void OP_Fx65();  // LD, Vx, I - read registers V0 through Vx from memory 
                     //   starting at location I.
};