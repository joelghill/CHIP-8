#include <iostream>
#include <fstream>
#include <string>

#include "chip-8.hpp"
#include "io.hpp"
#include "input/input_factory.hpp"
#include "input/mock_input.hpp"

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

    InputInterface* input = new MockInput();

    CHIP8* chip_8 = new CHIP8(input);

    // Load supplied rom
    vector<char>* rom_data = ReadRom(string(argv[1]));
    chip_8->LoadRom(rom_data);
    chip_8->Start();

    delete chip_8;
    return 0;
}