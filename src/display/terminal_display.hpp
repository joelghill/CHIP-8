#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include <iostream>
#include <ncurses.h>
#include "display_interface.hpp"
#include "../chip-8_state.hpp"

/**
 * @brief An interface used for updateing the emulator display
 *
 */
class TerminalDisplay : public DisplayInterface
{
public:
    /**
     * @brief Construct a new Terminal Display object
     *
     */
    TerminalDisplay();

    /**
     * @brief Destroy the Terminal Display object
     *
     */
    ~TerminalDisplay();

    /**
     * @brief Updates the emulator display using the provided display state
     *
     * @param display_state A byte array containing the current display data
     */
    virtual void updateDisplay(CHIP8_State* state);

private:

    WINDOW* window_;

};

#endif