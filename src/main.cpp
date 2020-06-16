#include <iostream>
#include <fstream>
#include <string>

#include "chip-8.hpp"
#include "io.hpp"
#include "input/sdl2_input.hpp"
#include "display/terminal_display.hpp"
#include "display/mock_display.hpp"

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

    // TerminalDisplay* display = new TerminalDisplay();
    MockDisplay* display = new MockDisplay();
    SDL2Input* input = new SDL2Input();

    CHIP8* chip_8 = new CHIP8(display, input);

    // Load supplied rom
    vector<char>* rom_data = ReadRom(string(argv[1]));
    chip_8->LoadRom(rom_data);
    chip_8->Start();

    delete chip_8;
    return 0;
}