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

using namespace std;

/**
 * @brief Construct a new CHIP8::CHIP8 object
 */
CHIP8::CHIP8() {

    // Initialize program counter at 0x200
    this->program_counter = 0x200;

    // Initialize the stack pointer to start at 0
    this->stack_pointer = 0;

    // Timers should be initialized to 0
    this->delay_timer = 0;
    this->sound_timer = 0;

    // Allocate memory for CHIP-8 RAM
    this->memory = new uint8_t[4096];

    // The uppermost 256 bytes (0xF00-0xFFF) are reserved for display refresh
    uint8_t* display = &this->memory[0xF00];

    // The 96 bytes below the display are reserved for the call stack
    // 0xEA0-0xEFF
    uint8_t* stack = &this->memory[0xEA0];

}

void CHIP8::LoadRom(uint8_t *rom) {

}