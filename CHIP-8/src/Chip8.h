#pragma once

#include <cstdint>

#define C8_CHARSET_SZ         0x50
#define C8_CHARSET_START_ADDR 0x00  // char set start address in memory
#define C8_CHARSET_END_ADDR   0x4F  // 0x50 chars in set, so max is 1 byte less



#define C8_START_ADDR         0x200 // accessible memory starts here

#define C8_DISPLAY_WIDTH  64  // width of display in pixels
#define C8_DISPLAY_HEIGHT 32  // height of display in pixels

class Chip8
{
public:
    Chip8();

    void Cycle();

    uint32_t* GetDisplay() { return display; }

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
    uint32_t display[C8_DISPLAY_WIDTH * C8_DISPLAY_HEIGHT]{};

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
    void LoadFunctionPointerTable();

    /* Opcode functions for unique opcodes. */
    void OP_1nnn();  // JP addr - jump to location nnn
    void OP_2nnn();  // CALL addr - call subroutine at location nnn
    void OP_3xkk();  // SE Vx, byte - skip next instruction if Vx == kk
    void OP_4xkk();  // SNE Vx, byte - skip next instruction if Vx != kk
    void OP_5xy0();  // SE Vx, Vy - skip next instruction if Vx == Vy
    void OP_6xkk();  // LD Vx, byte - set Vx = byte
    void OP_7xkk();  // ADD Vx, byte - set Vx = Vx + byte
    void OP_9xy0();  // SNE Vx, Vy - skip next instruction if Vx != Vy
    void OP_Annn();  // LD I, addr - set I = nnn
    void OP_Bnnn();  // JP V0, addr - jump to location nnn + V0
    void OP_Cxkk();  // RND Vx, byte - set Vx = random byte AND kk
    void OP_Dxyn();  // DRW Vx, Vy, nibble - display n-byte sprite starting at      
                     //   memory locationI at (Vx, Vy), set VF = collision

    /* Opcode functions for opcodes where they both begin with 00E but the
    last digit is unique. */
    void OP_00E0();  // CLS - clear the display
    void OP_00EE();  // RET - return from a subroutine

    /* Opcode functions for opcodes where the first digit repeats but the last
    digit is unique. */
    void OP_8xy0();  // LD Vx, Vy - set Vx = Vy
    void OP_8xy1();  // OR Vx, Vy - set Vx = Vx OR Vy
    void OP_8xy2();  // AND Vx, Vy - set Vx = Vx AND Vy
    void OP_8xy3();  // XOR Vx, Vy - set Vx = Vx XOR Vy
    void OP_8xy4();  // ADD Vx, Vy - set Vx = Vx + Vy, set VF = carry
    void OP_8xy5();  // SUB Vx, Vy - set Vx = Vx - Vy, set VF = NOT borrow
    void OP_8xy6();  // SHR Vx {, Vy} - set Vx = Vx SHR 1
    void OP_8xy7();  // SUBN Vx, Vy - set Vx - Vy - Vx, set VF = NOT borrow
    void OP_8xyE();  // SHL Vx, {, Vy} - set Vx = Vx SHL 1

    /* Opcode functions for opcodes where the first digit digit is E and the 
    last two digits are unique. */
    void OP_ExA1();  // SKNP Vx - skip next instruction if the key with the 
                     //   value Vx is not pressed.
    void OP_Ex9E();  // SKP Vx - skip next instruction if key with value Vx is
                     //   pressed

    /* Opcode functions for opcodes where the first digit is F and the last two
    digits are unique. */
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

    void OP_Null();  // blank opcode function.

    typedef void (Chip8::* OpFunc)();

    /* Unique opcodes range from 0 to F, so the table to store these requires
    F + 1 elements. */
    OpFunc optable[0xF + 1]{ &Chip8::OP_Null };

    /* Opcodes that begin with 00E have the last digit ranging from 0 to E, so
    the table requires E + 1 elements. */
    OpFunc optable0[0xE + 1]{ &Chip8::OP_Null };

    /* Opcodes that begin with 8 and have the last digit unique range from 0 to
    E, so the table requires E + 1 elements. */
    OpFunc optable8[0xE + 1]{ &Chip8::OP_Null };

    /* Opcodes beginning with E  have last digit values ranging from 0 
    to E, so the table store these requires E + 1 elements. */
    OpFunc optableE[0xE + 1]{ &Chip8::OP_Null };

    /* Opcodes that begin with F and ends with two unique digits range from 0
    to 65, so the table requires 65 + 1 elements. */
    OpFunc optableF[0x65 + 1]{ &Chip8::OP_Null };

    void Table0();
    void Table8();
    void TableE();
    void TableF();
};