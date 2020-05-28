/*

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
     * @brief
     *
     * @param rom A Byte array contaiing the rom data
     */
    void load_rom(uint8_t *rom);

private:
    // CHIP-8 VM has 4096 bytes of memory
    //
    uint8_t memory[4096];

    // The stack is an array of 16 16-bit values, used to store the address that the interpreter
    // should return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
    uint16_t stack[16];

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