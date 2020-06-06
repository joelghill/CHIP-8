#include <iostream>
#include <ncurses.h>
#include "terminal_display.hpp"
#include "../chip-8_state.hpp"

using namespace std;

TerminalDisplay::TerminalDisplay() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    this->window_ = newwin(DISPLAY_HEIGHT, DISPLAY_WIDTH, 1, 1);
}

TerminalDisplay::~TerminalDisplay() {
    endwin();
}

void TerminalDisplay::updateDisplay(CHIP8_State* state) {
    // clear the emulator window
    werase(this->window_);
    // for each byte in the display
    for (int j = 0; j < DISPLAY_HEIGHT; j++) {
        for (int i = 0; i < DISPLAY_WIDTH; i++) {
            bool is_pixel_active = state->displayValue(i, j);
            if (is_pixel_active) {
                mvwaddch(this->window_, j, i, ACS_BLOCK);
            } else {
                mvwaddch(this->window_, j, i, ' ');
            }
        }
    }
    //Refresh window to show changes
    wrefresh(this->window_);
}