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
#include "input/input_interface.hpp"
#include "display/display_interface.hpp"

using namespace std;

class CHIP8
{

public:

    /**
     * @brief Construct a new CHIP8 object. Null pointers will be initialized by the CHIP-8 constructor
     *
     * @param input Interface to a device input object
     * @param state (optional) The begining state of the CHIP-8 emulator
     */
    CHIP8(DisplayInterface* display, InputInterface* input, CHIP8_State* state=NULL);

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
     * @brief Reference to the interface to the emulator display
     *
     */
    DisplayInterface* display_;

    /**
     * @brief Reference to the interface used to retrieve hardware input
     *
     */
    InputInterface* input_;

    /**
     * @brief The internal state of the CHIP-8 emulator
     *
     */
    CHIP8_State* state_;
};

#endif