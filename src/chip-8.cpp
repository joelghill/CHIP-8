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
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using std::chrono::system_clock;

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

void CHIP8::LoadRom(vector<char> *rom) {
    for (int i = 0; i < rom->size(); i++) {
        this->memory[this->program_counter + i] = rom->at(i);
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
    return 1;
}