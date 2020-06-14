/**
 * @file op_codes.cpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-05-30
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include "chip-8_state.hpp"
#include "exceptions.hpp"
#include "op_codes.hpp"
#include "input/input_interface.hpp"
#include "display/display_interface.hpp"

uint8_t _getVxIndex(uint16_t op_code) {
    return (uint8_t)((op_code & 0x0F00) >> 8);
}

uint8_t _getVyIndex(uint16_t op_code) {
    return (uint8_t)((op_code & 0x00F0) >> 4);
}

int Execute00E0(CHIP8_State* state) {
    for (int j = 0; j < DISPLAY_HEIGHT; j++) {
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
            state->setDisplayValue(i, j, false);
        }
    }

    return DEFAULT_OP_CYCLES;
}

int Execute00EE(CHIP8_State* state) {
    uint16_t program_counter = state->popStack();
    state->setProgramCounter(program_counter);

    return DEFAULT_OP_CYCLES;
}

int Execute1NNN(CHIP8_State* state, uint16_t op_code) {
    // 1NNN - Jump to addres NNN
    int16_t jump_address = op_code & 0x0FFF;
    state->setProgramCounter(jump_address);

    return DEFAULT_OP_CYCLES;
}

int Execute2NNN(CHIP8_State* state, uint16_t op_code) {

    int16_t sub_routine = op_code & 0x0FFF;
    int16_t current_pc = state->programCounter();

    // Push the current PC onto the call stack
    state->pushStack(current_pc);
    // Then set the PC to the subroutine
    state->setProgramCounter(sub_routine);

    return DEFAULT_OP_CYCLES;
}

int Execute3XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    if (vx == nn) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute4XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    if (vx != nn) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute5XY0(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vy_index = _getVyIndex(op_code);

    uint8_t vx = state->vRegister(vx_index);
    uint8_t vy = state->vRegister(vy_index);
    if (vx == vy) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute6XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    state->setVRegister(vx_index, nn);

    return DEFAULT_OP_CYCLES;
}

int Execute7XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index) + nn;

    state->setVRegister(vx_index, vx);

    return DEFAULT_OP_CYCLES;
}

int Execute8XY0(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, vy);

    return DEFAULT_OP_CYCLES;
}

int Execute8XY1(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx | vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY2(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx & vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY3(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx ^ vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY4(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    // Add and save into 16bit to catch carry
    uint16_t addition = vx + vy;

    // Save the result as 8 bit
    state->setVRegister(vx_index, (uint8_t)addition);

    // Set VF register to 1 or 0 based on result
    if (addition > 255) {
        state->setVRegister(REGISTER_VF, 1);
    } else {
        state->setVRegister(REGISTER_VF, 0);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute8XY5(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    // Subtract and save into 16bit to catch carry
    uint8_t difference = vx - vy;

    // Save result as 8bits
    state->setVRegister(vx_index, (uint8_t)difference);

    // Set VF register to 1 or 0
    if (difference > vx) {
        // There was a borrow, set VF
        state->setVRegister(REGISTER_VF, 1);
    } else {
        state->setVRegister(REGISTER_VF, 0);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute8XY6(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    state->setVRegister(REGISTER_VF, vx & 0b0000'0001);
    state->setVRegister(vx_index, uint8_t(vx >> 1));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY7(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    // Set VF register to 1 if vy is greater than vxt
    if (vy > vx) {
        state->setVRegister(REGISTER_VF, 0);
    } else {
        state->setVRegister(REGISTER_VF, 1);
    }

    // Subtract vx from vy and save in vx
    uint8_t difference = vy - vx;

    // Save the result as 8 bit
    state->setVRegister(vx_index, difference);

    return DEFAULT_OP_CYCLES;
}

int Execute8XYE(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    // Save the most sig bit in VF
    state->setVRegister(REGISTER_VF, (uint8_t)((vx & 0b1000'0000) >> 7));
    state->setVRegister(vx_index, uint8_t(vx << 1));

    return DEFAULT_OP_CYCLES;
}

int Execute9XY0(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index);

    if (vx != vy) {
        uint16_t pc = state->programCounter();
        state->setProgramCounter(pc + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int ExecuteANNN(CHIP8_State* state, uint16_t op_code) {
    state->setIndexRegister(op_code & 0x0FFF);
    return DEFAULT_OP_CYCLES;
}

int ExecuteBNNN(CHIP8_State* state, uint16_t op_code) {
    uint8_t v0 = state->vRegister(0);
    state->setProgramCounter((op_code & 0x0FFF) + v0);
    return DEFAULT_OP_CYCLES;
}

int ExecuteCNNN(CHIP8_State* state, uint16_t op_code) {
    uint8_t random = rand() % 255;
    uint8_t vx_index = _getVxIndex(op_code);
    state->setVRegister(vx_index, (op_code & 0x00FF) & random);

    return DEFAULT_OP_CYCLES;
}

int ExecuteDXYN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index) % DISPLAY_WIDTH;

    uint8_t vy_index = _getVyIndex(op_code);
    uint8_t vy = state->vRegister(vy_index) % DISPLAY_HEIGHT;

    uint8_t sprite_height = (uint8_t)(op_code & 0x000F);
    uint8_t sprite_width = 8;

    bool changed_bit = false;
    uint16_t index_register = state->indexRegister();

    // For each row defining the sprite in data
    for (int row_index = 0; row_index < sprite_height; row_index++) {
        // Get the row of pixels
        uint8_t row_data = state->memoryValue(index_register + row_index);
        // Sprites are 8 pixels long, unless cut off by screen
        for (uint8_t pixel_index = 0; pixel_index < sprite_width; pixel_index++) {

            //Get the bit we want to check if it's set:
            bool sprite_bit = (row_data & (0b10000000 >> pixel_index)) > 0;

            int display_pixel_y = vy + row_index;
            if (display_pixel_y >= DISPLAY_HEIGHT) {
                continue;
            }

            int display_pixel_x =  vx + pixel_index;
            if (display_pixel_x >= DISPLAY_WIDTH) {
                continue;
            }

            bool display_bit = state->displayValue(display_pixel_x, display_pixel_y);

            // The new bit to display is the xor of the sprite bit and the current display bit
            bool new_display_bit = display_bit ^ sprite_bit;

            // If the display bit will change and the display bit is being cleared
            if (new_display_bit == false && display_bit == true) {
                // Set the flag that VF needs to be updated
                changed_bit = true;
            }

            // Set the bit based on the pixel offset
            state->setDisplayValue(display_pixel_x, display_pixel_y, new_display_bit);
        }
    }

    if (changed_bit) {
        state->setVRegister(0xF, 1);
    }
    else {
        state->setVRegister(0xF, 0);
    }

    return DEFAULT_OP_CYCLES;
}

int ExecuteEX9E(CHIP8_State* state, uint16_t op_code, InputInterface* input) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    if (input->isPressed(vx) == true) {
        // Skip next instruction by jumping PC ahead by 16 bits
        state->incrementProgramCounter(2);
    }

    return DEFAULT_OP_CYCLES;
}

int ExecuteEXA1(CHIP8_State* state, uint16_t op_code, InputInterface* input) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    if (input->isPressed(vx) == false) {
        // Skip next instruction by jumping PC ahead by 16 bits
        state->incrementProgramCounter(2);
    }

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX07(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t delay_timer = state->delayTimer();

    state->setVRegister(vx_index, delay_timer);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX0A(CHIP8_State* state, uint16_t op_code, InputInterface* input) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t key = input->getInput();

    state->setVRegister(vx_index, key);

    return BLOCKING_CALL;
}

int ExecuteFX15(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    state->setDelayTimer(vx);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX18(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    state->setSoundTimer(vx);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX1E(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    uint16_t index_register = state->indexRegister();

    uint16_t new_index_register = index_register + vx;
    state->setIndexRegister(new_index_register);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX29(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);
    uint16_t font_location = FONT_MEMORY_LOCATION + (uint16_t)(vx * 5);

    state->setIndexRegister(font_location);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX33(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint8_t vx = state->vRegister(vx_index);

    std::string vx_string = std::to_string(vx);
    uint8_t digits[] = {0 ,0 ,0};

    for (int i = 0; i < vx_string.length(); i++) {
        digits[i + (3 - vx_string.length())] = vx_string.at(i) - 48;
    }

    uint16_t index_register_address = state->indexRegister();
    state->setMemoryValue(index_register_address, digits[0]);
    state->setMemoryValue(index_register_address + 1, digits[1]);
    state->setMemoryValue(index_register_address + 2, digits[2]);

    return DEFAULT_OP_CYCLES;
}

int ExecuteFX55(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = _getVxIndex(op_code);
    uint16_t index_register_address = state->indexRegister();

    for (uint8_t i = 0; i <= vx_index; i++) {
        uint8_t v_value = state->vRegister(i);
        state->setMemoryValue((uint16_t)(index_register_address + i), v_value);
    }

    state->setIndexRegister(index_register_address + vx_index + 1);
    return DEFAULT_OP_CYCLES;
}

int ExecuteFX65(CHIP8_State* state, uint16_t op_code) {
    uint16_t vx_index = (uint16_t)_getVxIndex(op_code);
    uint16_t index_register_address = state->indexRegister();

    for (uint16_t i = 0; i <= vx_index; i++) {
        uint8_t v_value = state->memoryValue(index_register_address + i);
        state->setVRegister(i, v_value);
    }

    state->setIndexRegister(index_register_address + vx_index + 1);
    return DEFAULT_OP_CYCLES;
}