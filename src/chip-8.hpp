/**
 * @file chip-8.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2020-05-28
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef CHIP_8_H
#define CHIP_8_H

#include <iostream>
#include <vector>

using namespace std;

class CHIP8
{

public:

    /**
     * @brief Construct a new CHIP8 object. Null pointers will be initialized by the CHIP-8 constructor
     *
     * @param program_counter The current program counter. Defaults to 0x200
     * @param stack_pointer The stack pointer. Defaults to 0x000
     * @param v_registers Pointer to array containing the CHIP=8 registers. Defaults to NULL
     * @param memory Pointer to array containing CHIP-8 RAM. Defaults to NULL
     */
    CHIP8(
        int program_counter=0x200,
        uint16_t stack_pointer=0x00,
        uint8_t* v_registers=NULL,
        uint8_t* memory=NULL);

    /**
     * @brief Loads a CHIP-8 Rom into the emulator memory
     *
     * @param rom Byte array containing rom data
     */
    void LoadRom(vector<char> *rom);

    /**
     * @brief Begins emulation of CHIP-8
     *
     */
    void Start();

    /**
     * @brief Processes the 2 byte instruction and updates the state o the CHIP-8 emulator
     *
     * @param op_code A 2 byte instruction for the emulator to process
     * @return int The number of cycles required to process instruction
     */
    int ProcessOpCode(uint16_t op_code);

private:
    // CHIP-8 VM has 4096 bytes of memory
    uint8_t* memory;

    // The stack is an array of 16 16-bit values, used to store the address that the interpreter
    // should return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
    uint8_t* stack;

    // 64x32-pixel monochrome display
    uint8_t* display;

    // The current stack pointer
    uint16_t stack_pointer;

    // Registers
    // CHIP-8 has 16 8-bit data registers named V0 to VF
    // Each register is 8 bits
    uint8_t* v_registers;

    // 16bit index register
    uint16_t index_register;

    // Program counter, 16 bits
    uint16_t program_counter;

    // Delay and sound timers are 8 bits each
    // Delay timer is intended to be used for timing the events of games. Its value can be set and read
    uint8_t delay_timer;

    // When the sound timer value is nonzero, a beeping sound is made.
    uint8_t sound_timer;

    /**
     * @brief Emulates a cpu frame. Waits for emulated execution
     *
     */
    void ProcessCurrentFrame();

    /**
     * @brief Sets all bits in display RAM to zero
     *
     */
    void ClearDisplay();

    /**
     * @brief Pops address from the stack and sets to current program counter
     *
     */
    void ReturnFromSubroutine();

    /**
     * @brief Places the current program counter on the stack,
     * then sets the program counter to the provided address
     *
     * @param address The 16bit address of the sub-routine
     */
    void CallSubroutine(uint16_t address);

    /**
     * @brief Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
     * VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
     * and to 0 if that doesn’t happen
     *
     * @param x X coordinate of sprite
     * @param y Y coordinate of sprite
     * @param height Height of the sprite in pixels
     */
    void DrawSprite(uint8_t x, uint8_t y, uint8_t height);
};

#endif