#include <bitset>
#include <filesystem>
#include <iostream>
#include <sstream>
#include<vector>
#include "../src/chip-8_state.hpp"
#include "../src/op_codes.hpp"

using namespace std;

void printDisplay(CHIP8_State* state) {
    int display_memory_size = (DISPLAY_HEIGHT * DISPLAY_WIDTH) / 8;
    int row_size = DISPLAY_WIDTH / 8;
    int col = 0;
    for (int i = 0; i < display_memory_size; i++) {
        cout << bitset<8>(state->displayValue(i));
        col += 1;
        if (col == row_size) {
            col = 0;
            cout << endl;
        }
    }
}

int main(int argc, char** argv){

    uint8_t sprite[] = {
        0x90,
        0x90,
        0xF0,
        0x10,
        0x10
    };

    uint8_t* memory = new uint8_t[RAM_SIZE];
    uint8_t* v_registers = new uint8_t[15];
    v_registers[0] = 62;
    v_registers[1] = 3;


    for (int i = 0; i < 5; i++) {
        memory[512 + i] = sprite[i];
    }

    CHIP8_State* chip_8_state = new CHIP8_State(0x200, 0x200, 0, 1, v_registers, memory);
    ExecuteDXYN(chip_8_state, 0xD015);
    printDisplay(chip_8_state);

    delete chip_8_state;
    return 0;
}