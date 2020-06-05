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
#include "chip-8_state.hpp"
#include "exceptions.hpp"
#include "op_codes.hpp"
#include "input/input_interface.hpp"

int Execute00E0(CHIP8_State* state) {
    for (int i=0; i < (DISPLAY_HEIGHT * DISPLAY_WIDTH); i++) {
        state->setDisplayValue(i, 0x00);
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

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    if (vx == nn) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute4XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    if (vx != nn) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute5XY0(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vy_index = (uint8_t)(op_code & 0x00F0) >> 4;

    uint8_t vx = state->vRegister(vx_index);
    uint8_t vy = state->vRegister(vy_index);
    if (vx == vy) {
        state->setProgramCounter(state->programCounter() + 2);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute6XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    state->setVRegister(vx_index, nn);

    return DEFAULT_OP_CYCLES;
}

int Execute7XNN(CHIP8_State* state, uint16_t op_code) {

    uint8_t nn = (uint8_t)(op_code & 0x00FF);
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index) + nn;

    state->setVRegister(vx_index, vx);

    return DEFAULT_OP_CYCLES;
}

int Execute8XY0(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, vy);

    return DEFAULT_OP_CYCLES;
}

int Execute8XY1(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx | vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY2(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx & vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY3(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    state->setVRegister(vx_index, (vx ^ vy));

    return DEFAULT_OP_CYCLES;
}

int Execute8XY4(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
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
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    // Add and save into 16bit to catch carry
    uint16_t difference = vx - vy;

    // Set VF register to 1 or 0 based on result
    if (difference < 0) {
        state->setVRegister(REGISTER_VF, 0);
    } else {
        state->setVRegister(REGISTER_VF, 1);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute8XY6(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    state->setVRegister(REGISTER_VF, vx & 0b1000'0000);
    state->setVRegister(vx_index, vx << 1);

    return DEFAULT_OP_CYCLES;
}

int Execute8XY7(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    // Add and save into 16bit to catch carry
    uint16_t difference = vx - vy;

    // Save the result as 8 bit
    state->setVRegister(vx_index, (uint8_t)difference);

    // Set VF register to 1 or 0 based on result
    if (difference < 0) {
        state->setVRegister(REGISTER_VF, 0);
    } else {
        state->setVRegister(REGISTER_VF, 1);
    }

    return DEFAULT_OP_CYCLES;
}

int Execute8XYE(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    // Save the most sig bit in VF
    state->setVRegister(REGISTER_VF, vx & 0b1000'0000);
    state->setVRegister(vx_index, vx << 1);

    return DEFAULT_OP_CYCLES;
}

int Execute9XY0(CHIP8_State* state, uint16_t op_code) {
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
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
    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    state->setVRegister(vx_index, (op_code & 0x00FF) & random);

    return DEFAULT_OP_CYCLES;
}

int ExecuteDXYN(CHIP8_State* state, uint16_t op_code) {

    uint8_t vx_index = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t vx = state->vRegister(vx_index);

    uint8_t vy_index = (uint8_t)(op_code & 0x00f0) >> 4;
    uint8_t vy = state->vRegister(vy_index);

    uint8_t sprite_height = (uint8_t)(op_code & 0x000F);
    if (sprite_height + vy > DISPLAY_HEIGHT) {
        sprite_height = DISPLAY_HEIGHT - vy;
    }

    uint8_t sprite_width = 8;
    if ((vx + sprite_width) > DISPLAY_WIDTH) {
        sprite_width = DISPLAY_WIDTH - vx;
    }

    bool changed_bit = false;
    uint16_t index_register = state->indexRegister();

    for (int row_index = 0; row_index < sprite_height; row_index++) {
        // Get the row of pixels
        uint8_t row_data = state->memoryValue(index_register + row_index);
        // Sprites are 8 pixels long, unless cut off by screen
        for (uint8_t pixel_index = 0; pixel_index < sprite_width; pixel_index++) {

            //Get the bit we want to check if it's set:
            bool sprite_bit = (row_data & (0b10000000 >> pixel_index)) > 0;

            uint16_t bit_index = (DISPLAY_WIDTH * (vy + row_index)) + (vx + pixel_index);
            uint16_t byte_index = (bit_index / 8);
            uint16_t bit_offset = (bit_index % 8);

            uint8_t display_byte = state->displayValue(byte_index);
            bool display_bit = (display_byte & (0b10000000 >> bit_offset)) > 0;

            // If the display bit will change and the display bit is being cleared
            if ((display_bit != sprite_bit) && display_bit == true) {
                // Set the flag that VF needs to be updated
                changed_bit = true;
            }

            // If we're setting the bit
            if (sprite_bit == true) {
                // Set the bit based on the pixel offset
                display_byte = (display_byte | (0b10000000 >> bit_offset));
            } else {
                // Clear the bit based on the pixel offset
                display_byte = (display_byte & (~(0b10000000 >> bit_offset)));
            }

            state->setDisplayValue(byte_index, display_byte);
        }
    }

    if (changed_bit) {
        state->setVRegister(15, 1);
    }
    else {
        state->setVRegister(15, 0);
    }

    return DEFAULT_OP_CYCLES;
}