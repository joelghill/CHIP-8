#include <bitset>
#include <filesystem>
#include <iostream>
#include <sstream>
#include<vector>
#include "../src/chip-8_state.hpp"
#include "../src/op_codes.hpp"
#include "../src/display/terminal_display.hpp"

using namespace std;


int main(int argc, char** argv){

    TerminalDisplay* display = new TerminalDisplay();

    uint8_t sprite[] = {
        0x90,
        0x90,
        0xF0,
        0x10,
        0x10
    };

    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[15];
    v_registers[0] = 1;
    v_registers[1] = 5;

    for (int i = 0; i < 5; i++) {
        memory[0x200 + i] = sprite[i];
    }

    CHIP8_State* chip_8_state = new CHIP8_State(0x200, 0x200, 0, 1, v_registers, memory);
    ExecuteDXYN(chip_8_state, 0xD015);
    display->updateDisplay(chip_8_state);

    ExecuteDXYN(chip_8_state, 0xD115);
    display->updateDisplay(chip_8_state);

    delete chip_8_state;
    return 0;
}