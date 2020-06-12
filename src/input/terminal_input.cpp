#include <iostream>
#include <ncurses.h>
#include "terminal_input.hpp"

using namespace std;

TerminalInput::TerminalInput(WINDOW* window) {
    // each key the user hits is returned immediately by getch()
    cbreak();
    this->window_ = window;
    nodelay(this->window_, TRUE);
}

bool TerminalInput::isPressed(uint8_t input_code) {
    char key = input_code + 48; // ASCII offset
    char input;
    if ((input = wgetch(this->window_)) == ERR) {
        // There was no input
        return false;
    }

    return key == input;
}

uint8_t TerminalInput::getInput() {
    nodelay(this->window_, FALSE);
    char input = wgetch(this->window_);
    nodelay(this->window_, TRUE);

    return ((uint8_t)input) - 48;
}