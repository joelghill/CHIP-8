/**
 * @file chip-8.cpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief File containing the CHIP-8 emulator class implementation and supporting code
 * @date 2020-05-29
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <chrono>
#include <iostream>
#include <sstream>
#include<string>
#include <thread>
#include "chip-8.hpp"
#include "exceptions.cpp"

using namespace std;
using std::chrono::system_clock;

int DISPLAY_WIDTH = 64;
int DISPLAY_HEIGHT = 32;
int SPRITE_WIDTH = 8;

CHIP8::CHIP8(
        int program_counter,
        uint16_t stack_pointer,
        uint8_t* v_registers,
        uint8_t* memory){

    // Initialize program counter
    this->program_counter = program_counter;

    // Initialize the stack pointer
    this->stack_pointer = stack_pointer;

    // Timers should be initialized to 0
    this->delay_timer = 0;
    this->sound_timer = 0;

    // Initialize the V Registers
    if (v_registers == NULL) {
        this->v_registers = new uint8_t[16];
    } else {
        this->v_registers = v_registers;
    }

    // Initialize memory for CHIP-8 RAM
    if (memory == NULL) {
        this->memory = new uint8_t[4096];
    } else {
        this->memory = memory;
    }

    // The uppermost 256 bytes (0xF00-0xFFF) are reserved for display refresh
    this->display = &this->memory[0xF00];

    // The 96 bytes below the display are reserved for the call stack
    // 0xEA0-0xEFF
   this->stack = &(this->memory[0xEA0]);

}

void CHIP8::LoadRom(vector<char> *rom) {
    for (int i = 0; i < rom->size(); i++) {
        this->memory[this->program_counter + i] = rom->at(i);
    }
}

void CHIP8::Start() {

    while(true) {
        this->ProcessCurrentFrame();
    }
}

void CHIP8::ProcessCurrentFrame() {

    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

    // Get the time at the start or the frame
    system_clock::time_point frame_start_time = system_clock::now();

    // All instructions are 2 bytes long and are stored most-significant-byte first.
    uint8_t ms_op_code = this->memory[this->program_counter];
    uint8_t ls_op_code = this->memory[this->program_counter +1];

    // Use bit shift and bitwise operator to combine into single op code
    uint16_t op_code = ((uint16_t)ms_op_code << 8) | ls_op_code;
    this->program_counter = this->program_counter + 2;

    // Process the op code and record the number of CPU cycles used to process op code
    int cycles = this->ProcessOpCode(op_code);

    // The CHIP-8 processor runs at approximately 500Hz, which is 2ms per cycle
    // Wait time in milliseconds for the instruction
    int wait_time = 2 * cycles;

    // Get the time at the start or the frame
    system_clock::time_point frame_end = system_clock::now();
    system_clock::duration process_duration = frame_end - frame_start_time;

    // Force the thread to sleep for 2ms, minus the time it took to process the op code
    sleep_for(2ms - process_duration);
}

int CHIP8::ProcessOpCode(uint16_t op_code) {
    uint8_t nyble_1 = (uint8_t)((op_code & 0xF000) >> 12);
    uint8_t nyble_2 = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t nyble_3 = (uint8_t)(op_code & 0x00F0) >> 4;
    uint8_t nyble_4 = (uint8_t)(op_code & 0x000F);

    switch(nyble_1) {
        case 0x00:
            if (op_code == 0x00E0) {
                // Clear the screen
                this->ClearDisplay();
                return 1;

            } else if (op_code == 0x00EE) {
                // Return from subroutine
                this->ReturnFromSubroutine();
                return 1;
            }
            else {
                // unused
                return 1;
            }

        case 0x01: {
            // 1NNN - Jump to addres NNN
            int16_t jump_address = op_code & 0x0FFF;
            this->program_counter = jump_address;
            return 1;
        }

        case 0x02: {
            // 2NNN - Call subroutine at NNN
            int16_t sr_address = op_code & 0x0FFF;
            this->CallSubroutine(sr_address);
            return 1;
        }

        case 0x03: {
            // 3XNN - Skips the next instruction if VX equals NN
            uint8_t vx = this->v_registers[nyble_2];
            uint8_t nn = (uint8_t)(op_code & 0x00FF);
            if (vx == nn) {
                this->program_counter += 2;
            }
            return 1;
        }

        case 0x04: {
            // 4XNN - Skips the next instruction if VX does not equal NN
            uint8_t vx = this->v_registers[nyble_2];
            uint8_t nn = (uint8_t)(op_code & 0x00FF);
            if (vx != nn) {
                this->program_counter += 2;
            }
            return 1;
        }

        case 0x05: {
            // 5XY0 - Skips the next instruction if VX equals VY.
            uint8_t vx = this->v_registers[nyble_2];
            uint8_t vy = this->v_registers[nyble_3];
            if (vx == vy) {
                this->program_counter += 2;
            }
            return 1;
        }

        case 0x06: {
            // 6XNN - Sets VX to NN
            uint8_t nn = (uint8_t)(op_code & 0x00FF);
            this->v_registers[nyble_2] = nn;
            return 1;
        }

        case 0x07: {
            // 7XNN - Adds NN to VX.
            uint8_t nn = (uint8_t)(op_code & 0x00FF);
            this->v_registers[nyble_2] += nn;
            return 1;
        }

        // 8XYN
        case 0x08: {
            switch (nyble_4) {

                case 0x00: {
                    // 8XY0 - Sets VX to the value of VY.
                    uint8_t vy = this->v_registers[nyble_3];
                    this->v_registers[nyble_2] = vy;
                    return 1;
                }

                case 0x01: {
                    // 8XY1 - Sets VX to VX or VY.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];
                    this->v_registers[nyble_2] = vx | vy;
                    return 1;
                }

                case 0x02: {
                    // 8XY2 - Sets VX to VX and VY.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];
                    this->v_registers[nyble_2] = vx & vy;
                    return 1;
                }

                case 0x03: {
                    // 8XY3 - Sets VX to VX xor VY.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];
                    this->v_registers[nyble_2] = vx ^ vy;
                    return 1;
                }

                case 0x04: {
                    // 8XY4 - Adds VY to VX.
                    // VF is set to 1 when there's a carry, and to 0 when there isn't.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];

                    // Add and save into 16bit to catch carry
                    uint16_t addition = vx + vy;

                    // Save the result as 8bit
                    this->v_registers[nyble_2] = (uint8_t)addition;

                    // Set VF register to 1 or 0 based on result
                    if (addition > 255) {
                        this->v_registers[15] = 1;
                    } else {
                        this->v_registers[15] = 0;
                    }
                    return 1;
                }

                case 0x05: {
                    // 8XY5 - VY is subtracted from VX.
                    // VF is set to 0 when there's a borrow, and 1 when there isn't.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];

                    // Add and save into 16bit to catch carry
                    int16_t difference = vx - vy;

                    // Save the result as 8bit
                    this->v_registers[nyble_2] = (uint8_t)difference;

                    // Set VF register to 1 or 0 based on result
                    if (difference < 0) {
                        this->v_registers[15] = 0;
                    } else {
                        this->v_registers[15] = 1;
                    }
                    return 1;
                }

                case 0x06: {
                    // 8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
                    uint8_t vx = this->v_registers[nyble_2];

                    // Save the result as 8bit
                    this->v_registers[15] = vx & 0b0000'0001;
                    this->v_registers[nyble_2]  = vx >> 1;
                    return 1;
                }

                case 0x07: {
                    // 8XY7 - Vx is subtracted from Vy.
                    // VF is set to 0 when there's a borrow, and 1 when there isn't.
                    uint8_t vx = this->v_registers[nyble_2];
                    uint8_t vy = this->v_registers[nyble_3];

                    // Add and save into 16bit to catch carry
                    int16_t difference = vy - vx;

                    // Save the result as 8bit
                    this->v_registers[nyble_2] = (uint8_t)difference;

                    // Set VF register to 1 or 0 based on result
                    if (difference < 0) {
                        this->v_registers[15] = 0;
                    } else {
                        this->v_registers[15] = 1;
                    }
                    return 1;
                }

                case 0x0E: {
                    // 8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
                    uint8_t vx = this->v_registers[nyble_2];

                    // Save the most sig bit in VF
                    this->v_registers[15] = vx & 0b1000'0000;
                    this->v_registers[nyble_2]  = vx << 1;
                    return 1;
                }
            }

            case 0x09: {
                // 9XY0 - Skips the next instruction if VX doesn't equal VY.
                // (Usually the next instruction is a jump to skip a code block)
                uint8_t vx = this->v_registers[nyble_2];
                uint8_t vy = this->v_registers[nyble_3];

                if (vx != vy) {
                    this->program_counter += 2;
                }
                return 1;
            }

            case 0x0A: {
                // ANNN - Sets I to the address NNN.
                this->index_register = (op_code & 0x0FFF);
                return 1;
            }

            case 0x0B: {
                // BNNN - Jumps to the address NNN plus V0.
                uint8_t v0 = this->v_registers[0];
                this->program_counter = (op_code & 0x0FFF) + v0;
                return 1;
            }

            case 0x0C: {
                // CXNN - Sets VX to the result of a bitwise and operation on a random number and NN
                // (Typically: 0 to 255)
                uint8_t random = rand() % 255;
                this->v_registers[nyble_2] = ((op_code & 0x00FF) & random);
                return 1;
            }
        }

        default: {
            stringstream error_string;
            error_string << "Op Code " << op_code << " has not yet been implemented." << endl;
            throw OperationNotImplementedException(error_string.str());
        }
    }

    return 1;
}

void CHIP8::ClearDisplay() {
    for (int i=0; i < (DISPLAY_HEIGHT * DISPLAY_WIDTH); i++) {
        this->display[i] = 0x00;
    }
}

void CHIP8::ReturnFromSubroutine() {
    this->program_counter = this->stack[this->stack_pointer];
    if (this->stack_pointer > 0x0000) {
        this->stack_pointer--;
    }
}

void CHIP8::CallSubroutine(uint16_t address) {
    if (this->stack_pointer > 0x0000) {
        this->stack_pointer++;
    }

    this->stack[this->stack_pointer] = this->program_counter;
    this->program_counter = address;
}

void CHIP8::DrawSprite(uint8_t x, uint8_t y, uint8_t height) {

    // Get the index of the sprite in memory
    uint8_t memory_index = x + (y * SPRITE_WIDTH);

    // for each row of the sprite
    for (int i=this->index_register; i < this->index_register + height; i++) {
        uint8_t sprite_row = this->memory[i];
        for ()
    }
}