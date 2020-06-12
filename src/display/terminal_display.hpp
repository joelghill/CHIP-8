#ifndef TERMINAL_DISPLAY_H
#define TERMINAL_DISPLAY_H

#include <iostream>
#include <ncurses.h>
#include "display_interface.hpp"
#include "../chip-8_state.hpp"

const static int TOP_PADDING = 5;
const static int BOTTOM_PADDING = 5;
const static int LEFT_PADDING = 5;
const static int RIGHT_PADDING = 5;

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
     * @param window The terminal window
     */
    TerminalDisplay();

    /**
     * @brief Destroy the Terminal Display object
     *
     */
    ~TerminalDisplay() = default;

    /**
     * @brief Updates the emulator display using the provided display state
     *
     * @param display_state A byte array containing the current display data
     */
    virtual void updateDisplay(CHIP8_State* state);

    /**
     * @brief Gets the Window object
     *
     * @return WINDOW*
     */
    WINDOW* getWindow();

private:

    WINDOW* window_;

};

#endif