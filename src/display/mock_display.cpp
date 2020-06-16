#include "mock_display.hpp"
#include "../chip-8_state.hpp"

using namespace std;

MockDisplay::MockDisplay() { }

void MockDisplay::updateDisplay(CHIP8_State* state) {
    // clear the emulator window
    // for each byte in the display
    for (int j = 0; j < DISPLAY_HEIGHT; j++) {
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
            bool is_pixel_active = state->displayValue(i, j);
            if (is_pixel_active) {
                cout << "#";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}