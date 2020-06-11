/**
 * @file chip-8_state.hpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-05-30
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef CHIP_8_STATE_H
#define CHIP_8_STATE_H

#include <iostream>

using namespace std;

static int V_REGISTER_COUNT = 16;
static int RAM_SIZE = 4096;

static uint16_t INITAL_PROGRAM_COUNTER = 0x200;
static uint16_t DISPLAY_MEMORY_LOCATION = 0xF00;
static uint16_t STACK_MEMORY_LOCATION = 0xEA0;
static uint16_t FONT_MEMORY_LOCATION = 0x0;

class CHIP8_State
{

public:

    /**
     * @brief Construct a new chip8 state object
     *
     * @param programCounter
     * @param indexRegister
     * @param delayTimer
     * @param soundTimer
     * @param vRegisters
     * @param memory
     */
    CHIP8_State(
        int programCounter=INITAL_PROGRAM_COUNTER,
        uint16_t indexRegister=0,
        uint8_t delayTimer=0,
        uint8_t soundTimer=0,
        uint8_t* vRegisters=NULL,
        uint8_t* memory=NULL);

private:

    // CHIP-8 VM has 4096 bytes of memory
    uint8_t* memory_;

    // The stack is an array of 16 16-bit values, used to store the address that the interpreter
    // should return to when finished with a subroutine. Chip-8 allows for up to 16 levels of nested subroutines.
    uint8_t* stack_;

    // 64x32-pixel monochrome display
    // Represented as an array of bool arrays
    bool display_[64][32];

    // The current stack pointer
    int16_t stackPointer_ = -2;

    // Registers
    // CHIP-8 has 16 8-bit data registers named V0 to VF
    // Each register is 8 bits
    uint8_t* vRegisters_;

    // 16bit index register
    uint16_t indexRegister_;

    // Program counter, 16 bits
    uint16_t programCounter_;

    // Delay and sound timers are 8 bits each
    // Delay timer is intended to be used for timing the events of games. Its value can be set and read
    uint8_t delayTimer_;

    // When the sound timer value is nonzero, a beeping sound is made.
    uint8_t soundTimer_;

    // Hard coded definition of CHIP-8 font set
    uint8_t fontset_[80] = {
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

public:

    /**
     * @brief Pushes a new addres onto the call stack of the CHIP-8 State
     *
     * @param address The new address to push onto the call stack.
     */
    void pushStack(uint16_t address);

    /**
     * @brief Pops the top memory address from the stack
     *
     * @return uint8_t The memory address that was stored on top of the stack
     */
    uint16_t popStack();

    /**
     * @brief Reads the current top value in the stack without modifying the stack
     *
     * @return uint16_t The current stack value
     */
    uint16_t peekStack();

    /**
     * @brief The index of the current stack pointer
     *
     * @return uint16_t Current stack pointer, 0 - 15
     */
    int16_t stackPointer();

    /**
     * @brief Gets the value of one of the 16 registers
     *
     * @param register_index Index from 0x00 to 0x0F
     * @return uint8_t The value stored in the register
     */
    uint8_t vRegister(uint8_t register_index);

    /**
     * @brief Sets the value of one of the 16 registers
     *
     * @param register_index Index from 0x00 to 0x0F
     * @param value The 8bit value to save in the register
     */
    void setVRegister(uint8_t register_index, uint8_t value);

    /**
     * @brief Gets the current value for the dedicated "I" register
     *
     * @return uint16_t The value for register "I"
     */
    uint16_t indexRegister();

    /**
     * @brief Sets the Index Register to the provided value
     *
     * @param value 16bit value to assign to the "I" register
     */
    void setIndexRegister(uint16_t value);

    /**
     * @brief Gets the current value of the CHIP-8 program counter "PC"
     *
     * @return uint16_t The current program counter.
     */
    uint16_t programCounter();

    /**
     * @brief Sets the Program Counter
     *
     * @param value The new PC
     */
    void setProgramCounter(uint16_t value);

    /**
     * @brief Increments the program counter by the provided integer
     *
     * @param value A positive or negative integer
     */
    void incrementProgramCounter(int value);

    /**
     * @brief Gets the current value of the delay timer
     *
     * @return uint8_t The delay timer value
     */
    uint8_t delayTimer();

    /**
     * @brief Sets the Delay Timer
     *
     * @param value The new delay timer value
     */
    void setDelayTimer(uint8_t value);

    /**
     * @brief Gets the current sound timer value
     *
     * @return uint8_t THe Sound timer value
     */
    uint8_t soundTimer();

    /**
     * @brief Sets the Sound Timer
     *
     * @param value The new sound timer value
     */
    void  setSoundTimer(uint8_t value);

    /**
     * @brief Gets the value stored in memory at the provided address
     *
     * @param index The memory address
     * @return uint8_t The value in memory stored at the provided address
     */
    uint8_t memoryValue(uint16_t index);

    /**
     * @brief Sets a value into memory at the index provided
     *
     * @param index The memory address
     * @param value The value to set into memory
     */
    void setMemoryValue(uint16_t index, uint8_t value);

    /**
     * @brief Gets the value of a display byte at a specified memory location
     *
     * @param index The index of the display byte
     * @return uint8_t The value stored in the display memory at the specified index
     */
    bool displayValue(int x, int y);

    /**
     * @brief Sets the byte in the display memory at the provided location
     *
     * @param index The index of the byte to set
     * @param value The value to set in the display memory
     */
    void setDisplayValue(int x, int y, bool value);
};

#endif