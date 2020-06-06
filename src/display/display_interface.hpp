#ifndef DISPLAY_INTERFACE_H
#define DISPLAY_INTERFACE_H

#include <iostream>
#include "../chip-8_state.hpp"

const static int DISPLAY_WIDTH = 64;
const static int DISPLAY_HEIGHT = 32;
static int SPRITE_WIDTH = 8;

/**
 * @brief An interface used for updateing the emulator display
 *
 */
class DisplayInterface
{
public:
    /**
     * @brief Destroy the Input Interface object
     *
     */
    virtual ~DisplayInterface() = default;

    /**
     * @brief Updates the emulator display using the provided display state
     *
     * @param display_state A byte array containing the current display data
     */
    virtual void updateDisplay(CHIP8_State* state) = 0;

};

#endif