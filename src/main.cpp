#include "chip-8.hpp"
#include "io.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/**
 * @brief Main executable entry point
 *
 * @return int
 */
int main(int argc, char** argv){

    if (argc < 2)
    {
        cout << "A ROM file path must be supplied" << endl;
        return -1;
    }

    CHIP8* chip_8 = new CHIP8();

    // Load supplied rom
    vector<char>* rom_data = ReadRom(string(argv[1]));
    chip_8->LoadRom(rom_data);

    delete chip_8;
    return 0;
}