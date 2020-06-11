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
#include <string>
#include <thread>
#include "chip-8.hpp"
#include "chip-8_state.hpp"
#include "exceptions.hpp"
#include "op_codes.hpp"
#include "input/input_interface.hpp"

using namespace std;
using std::chrono::system_clock;

CHIP8::CHIP8(DisplayInterface* display, InputInterface* input, CHIP8_State* state) {
    // Set the display interface
    this->display_ = display;

    // Set the input instance
    this->input_ = input;

    // Set to provided state, or instantiate own if none provided
    if (state == NULL) {
        this->state_ = new CHIP8_State();
    } else {
        this->state_ = state;
    }
}

void CHIP8::LoadRom(vector<char> *rom) {
    for (int i = 0; i < rom->size(); i++) {
        this->state_->setMemoryValue(INITAL_PROGRAM_COUNTER + i, rom->at(i));
    }
}

void CHIP8::Start() {
    using namespace std::this_thread;     // sleep_for, sleep_until
    using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

    int timer_counter = 0;
    while(true) {

        // Get the time at the start or the frame
        system_clock::time_point frame_start_time = system_clock::now();

        // Process the frame
        int cycles = this->ProcessCurrentFrame();

        // The CHIP-8 processor runs at approximately 500Hz, which is 2ms per cycle
        // Wait time in milliseconds for the instruction
        int wait_time = 2 * cycles;

        // Update timers
        timer_counter ++;
        if (timer_counter > 7) {
            timer_counter = 0;

            // The timers run at 60Hz, so around every 8 frames
            uint8_t delay_time = this->state_->delayTimer();
            if (delay_time > 0) {
                this->state_->setDelayTimer(delay_time - 1);
            }

            uint8_t sound_time = this->state_->soundTimer();
            if (sound_time > 0) {
                this->state_->setSoundTimer(sound_time - 1);
            }
        }

        // Get the time at the start or the frame
        system_clock::time_point frame_end = system_clock::now();
        system_clock::duration process_duration = frame_end - frame_start_time;

        // Force the thread to sleep for 2ms, minus the time it took to process the op code
        sleep_for(2ms - process_duration);
    }
}

int CHIP8::ProcessCurrentFrame() {
     // Get current PC
    uint16_t current_pc = this->state_->programCounter();

    // All instructions are 2 bytes long and are stored most-significant-byte first.
    uint8_t ms_op_code = this->state_->memoryValue(current_pc);
    uint8_t ls_op_code = this->state_->memoryValue(current_pc + 1);

    // Use bit shift and bitwise operator to combine into single op code
    uint16_t op_code = ((uint16_t)ms_op_code << 8) | ls_op_code;

    // Move program counter forward 16 bits
    this->state_->setProgramCounter(current_pc + 2);

    // Process the op code and return the number of CPU cycles used to process op code
    int cycles = this->ProcessOpCode(op_code);

    // After everthing has been processed, update the display
    this->display_->updateDisplay(this->state_);

    return cycles;
}

int CHIP8::ProcessOpCode(uint16_t op_code) {
    uint8_t nyble_1 = (uint8_t)((op_code & 0xF000) >> 12);
    uint8_t nyble_2 = (uint8_t)(op_code & 0x0F00) >> 8;
    uint8_t nyble_3 = (uint8_t)(op_code & 0x00F0) >> 4;
    uint8_t nyble_4 = (uint8_t)(op_code & 0x000F);

    switch(nyble_1) {
        case 0x00:
            if (op_code == 0x00E0) {
                return Execute00E0(this->state_);

            } else if (op_code == 0x00EE) {
                // Return from subroutine
                return Execute00EE(this->state_);
            }
            else {
                // unused
                return 1;
            }

        case 0x01: {
            // 1NNN - Jump to addres NNN
            return Execute1NNN(this->state_, op_code);
        }

        case 0x02: {
            // 2NNN - Call subroutine at NNN
            return Execute2NNN(this->state_, op_code);
        }

        case 0x03: {
            // 3XNN - Skips the next instruction if VX equals NN
            return Execute3XNN(this->state_, op_code);
        }

        case 0x04: {
            // 4XNN - Skips the next instruction if VX does not equal NN
            return Execute4XNN(this->state_, op_code);
        }

        case 0x05: {
            // 5XY0 - Skips the next instruction if VX equals VY.
            return Execute5XY0(this->state_, op_code);
        }

        case 0x06: {
            // 6XNN - Sets VX to NN
            return Execute6XNN(this->state_, op_code);
        }

        case 0x07: {
            // 7XNN - Adds NN to VX. (Carry flag is not changed)
            return Execute7XNN(this->state_, op_code);
        }

        // 8XYN
        case 0x08: {
            switch (nyble_4) {

                case 0x00: {
                    // 8XY0 - Sets VX to the value of VY.
                    return Execute8XY0(this->state_, op_code);
                }

                case 0x01: {
                    // 8XY1 - Sets VX to VX or VY.
                    return Execute8XY1(this->state_, op_code);
                }

                case 0x02: {
                    // 8XY2 - Sets VX to VX and VY.
                    return Execute8XY2(this->state_, op_code);
                }

                case 0x03: {
                    // 8XY3 - Sets VX to VX xor VY.
                    return Execute8XY3(this->state_, op_code);
                }

                case 0x04: {
                    // 8XY4 - Adds VY to VX.
                    // VF is set to 1 when there's a carry, and to 0 when there isn't.
                    return Execute8XY4(this->state_, op_code);
                }

                case 0x05: {
                    // 8XY5 - VY is subtracted from VX.
                    // VF is set to 0 when there's a borrow, and 1 when there isn't.
                    return Execute8XY5(this->state_, op_code);
                }

                case 0x06: {
                    // 8XY6 - Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
                    return Execute8XY6(this->state_, op_code);
                }

                case 0x07: {
                    // 8XY7 - Vx is subtracted from Vy.
                    // VF is set to 0 when there's a borrow, and 1 when there isn't.
                    return Execute8XY7(this->state_, op_code);
                }

                case 0x0E: {
                    // 8XYE - Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
                    return Execute8XYE(this->state_, op_code);
                }
            }

            case 0x09: {
                // 9XY0 - Skips the next instruction if VX doesn't equal VY.
                // (Usually the next instruction is a jump to skip a code block)
                return Execute9XY0(this->state_, op_code);
            }

            case 0x0A: {
                // ANNN - Sets I to the address NNN.
                return ExecuteANNN(this->state_, op_code);
            }

            case 0x0B: {
                // BNNN - Jumps to the address NNN plus V0.
                return ExecuteBNNN(this->state_, op_code);
            }

            case 0x0C: {
                // CXNN - Sets VX to the result of a bitwise and operation on a random number and NN
                // (Typically: 0 to 255)
                return ExecuteCNNN(this->state_, op_code);
            }

            case 0x0D: {
                // DXYN - Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
                return ExecuteDXYN(this->state_, op_code);
            }

            case 0x0E: {
                // There are two op codes we are concerned about  when the first nibble is 0x0E
                uint8_t e_op_code = op_code & 0x00FF;
                if (e_op_code == 0x9E) {
                    // EX9E - Skips the next instruction if the key stored in VX is pressed.
                    // (Usually the next instruction is a jump to skip a code block)
                    return ExecuteEX9E(this->state_, op_code, this->input_);
                }
                else if (e_op_code == 0xA1) {
                    // EXA1 - Skips the next instruction if the key stored in VX isn't pressed.
                    // (Usually the next instruction is a jump to skip a code block)
                    return ExecuteEXA1(this->state_, op_code, this->input_);
                }
                else {
                    stringstream error_string;
                    error_string << "Op Code " << op_code << " has not yet been implemented." << endl;
                    throw OperationNotImplementedException(error_string.str());
                }
            }

            case 0x0F: {
                // There are two op codes we are concerned about  when the first nibble is 0x0E
                uint8_t f_op_code = op_code & 0x00FF;

                switch(f_op_code) {
                    case 0x07: {
                        // 0xFX07 -Sets VX to the value of the delay timer.
                        return ExecuteFX07(this->state_, op_code);
                    }
                    case 0x0A: {
                        // 0xFX0A - A key press is awaited, and then stored in VX.
                        return ExecuteFX0A(this->state_, op_code, this->input_);
                    }
                    case 0x15: {
                        // 0xFX15 - Sets the delay timer to VX
                        return ExecuteFX15(this->state_, op_code);
                    }
                    case 0x18: {
                        // 0xFX18 - Sets the sound timer to VX.
                        return ExecuteFX18(this->state_, op_code);
                    }
                    case 0x1E: {
                        // 0xFX1E - Adds VX to I.
                        // VF is set to 1 when there is a range overflow (I+VX>0xFFF), and to 0 when there isn't.
                        return ExecuteFX1E(this->state_, op_code);
                    }
                    case 0x29: {
                        // 0xFX29 - Sets I to the location of the sprite for the character in VX.
                        return ExecuteFX29(this->state_, op_code);
                    }
                    case 0x33: {
                        // 0xFX33 - Stores the binary-coded decimal representation of VX,
                        // with the most significant of three digits at the address in I
                        return ExecuteFX33(this->state_, op_code);
                    }
                    case 0x55: {
                        // 0xFX55 - Stores V0 to VX (including VX) in memory starting at address I.
                        return ExecuteFX55(this->state_, op_code);
                    }
                    case 0x65: {
                        // 0xFX65 - Fills V0 to VX (including VX) with values from memory starting at address I.
                        return ExecuteFX65(this->state_, op_code);
                    }
                }
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