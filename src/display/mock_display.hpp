#ifndef MOCK_DISPLAY_H
#define MOCK_DISPLAY_H

#include "display_interface.hpp"
#include "../chip-8_state.hpp"

/**
 * @brief An interface used for updateing the emulator display
 *
 */
class MockDisplay : public DisplayInterface
{
public:
    /**
     * @brief Construct a new Terminal Display object
     *
     * @param window The terminal window
     */
    MockDisplay();

    /**
     * @brief Destroy the Terminal Display object
     *
     */
    ~MockDisplay() = default;

    /**
     * @brief Updates the emulator display using the provided display state
     *
     * @param display_state A byte array containing the current display data
     */
    virtual void updateDisplay(CHIP8_State* state);
};

#endif