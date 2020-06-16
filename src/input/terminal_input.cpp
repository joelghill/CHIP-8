#include <exception>
#include <iostream>
#include <map>
#include <ncurses.h>
#include <stdio.h>
#include <thread>
#include "terminal_input.hpp"

using namespace std;

TerminalInput::TerminalInput(WINDOW* window) {
    // each key the user hits is returned immediately by getch()
    cbreak();
    noecho();

    this->window_ = window;
    nodelay(this->window_, TRUE);

    this->input_thread_ = new thread(&TerminalInput::updateInputState, this);
}

TerminalInput::~TerminalInput() {
    this->input_thread_->join();
}

bool TerminalInput::isPressed(uint8_t input_code) {

    if (this->input_stale) {
        // No input since last check, return false
        return false;
    } else {
        // otherwise check for input match
        bool is_pressed = input_code == this->last_keypress;
        return is_pressed;
    }
}

uint8_t TerminalInput::getInput() {
    // Wait for input thread to release lock
    this->read_input_mutext_.lock();
    uint8_t input = this->last_keypress;
    this->read_input_mutext_.unlock();

    return input;
}

void TerminalInput::updateInputState() {
    map<char, uint8_t> char_to_code {
        {'0', 0x0},
        {'1', 0x1},
        {'2', 0x2},
        {'3', 0x3},
        {'4', 0x4},
        {'5', 0x5},
        {'6', 0x6},
        {'7', 0x7},
        {'8', 0x8},
        {'9', 0x9},
        {'a', 0xa},
        {'b', 0xb},
        {'c', 0xc},
        {'d', 0xd},
        {'e', 0xe},
        {'f', 0xf},
    };

    while(true) {
        // Enter critical section to update input
        this->read_input_mutext_.lock();
        // wait for input
        this->input_stale = true;
        char input = getchar();
        // Flush input that accumulated during wait time
        // Do not want the input buffer to accumulate
        fflush(stdin);
        this->last_keypress = char_to_code[input];
        // leave critical section
        this->read_input_mutext_.unlock();

        // Update the flag indicating new input has been set
        this->input_stale = false;

        // Sleep for a short amount of time before marking input as stale
        this_thread::sleep_for (chrono::milliseconds(100));
    }
}