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
#include "chip-8_state.hpp"

using namespace std;

class CHIP8
{

public:

    /**
     * @brief Construct a new CHIP8 object. Null pointers will be initialized by the CHIP-8 constructor
     *
     * @param state (optional) The begining state of the CHIP-8 emulator
     */
    CHIP8(CHIP8_State* state=NULL);

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
     * @brief Moves the CHIP-8 state a single frame forward
     *
     * @return int The number of cycles required to process the frame
     */
    int ProcessCurrentFrame();

    /**
     * @brief Processes the 2 byte instruction and updates the state o the CHIP-8 emulator
     *
     * @param op_code A 2 byte instruction for the emulator to process
     * @return int The number of cycles required to process instruction
     */
    int ProcessOpCode(uint16_t op_code);

private:

    /**
     * @brief The internal state of the CHIP-8 emulator
     *
     */
    CHIP8_State* state_;


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