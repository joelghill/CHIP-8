#include "chip-8.hpp"
#include <iostream>

/**
 * @brief Construct a new CHIP8::CHIP8 object
 */
CHIP8::CHIP8() {

    // Initialize the stack pointer to start at 0
    this->stack_pointer = 0;
}