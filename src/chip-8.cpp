/**
 * @file chip-8.cpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief File containing the CHIP-8 emulator class implementation and supporting code
 * @date 2020-05-29
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "chip-8.hpp"
#include <iostream>

/**
 * @brief Construct a new CHIP8::CHIP8 object
 */
CHIP8::CHIP8() {

    // Initialize program counter at 0x200
    this->program_counter = 0x200;

    // Initialize the stack pointer to start at 0
    this->stack_pointer = 0;
}

void CHIP8::LoadRom(uint8_t *rom) {

}