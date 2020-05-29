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

class CHIP8
{


public:

    /**
     * @brief Construct a new CHIP8 object
     *
     */
    CHIP8();

    /**
     * @brief Loads a CHIP-8 Rom into the emulator memory
     *
     * @param rom Byte array containing rom data
     */
    void LoadRom(uint8_t *rom);

private:
    // CHIP-8 VM has 4096 bytes of memory
    uint8_t* memory;

    // The stack is an array of 16 16-bit values, used to store the address that the interpreter
    // should return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
    uint8_t* stack;

    // 64x32-pixel monochrome display
    uint8_t* display;

    // The current stack pointer
    int stack_pointer;

    // Registers
    // CHIP-8 has 16 8-bit data registers named V0 to VF
    // Each register is 8 bits
    uint8_t v[16];

    // 16bit index register
    uint16_t index_register;

    // Program counter, 16 bits
    uint16_t program_counter;

    // Delay and sound timers are 8 bits each
    // Delay timer is intended to be used for timing the events of games. Its value can be set and read
    uint8_t delay_timer;

    // When the sound timer value is nonzero, a beeping sound is made.
    uint8_t sound_timer;
};

#endif