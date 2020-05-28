#include "chip-8.hpp"
#include <iostream>

/**
 * @brief Main executable entry point
 *
 * @return int
 */
int main(){
    CHIP8* chip_8 = new CHIP8();


    delete chip_8;
    return 0;
}