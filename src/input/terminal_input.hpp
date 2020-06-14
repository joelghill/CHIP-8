#ifndef TERMINAL_INPUT_H
#define TERMINAL_INPUT_H

#include <iostream>
#include <map>
#include <mutex>
#include <ncurses.h>
#include <thread>
#include "input_interface.hpp"

using namespace std;

/**
 * @brief An implementation of the InputInterface that uses the terminal for keyboard input
 *
 */
class TerminalInput : public InputInterface
{

public:

    /**
     * @brief Construct a new Terminal Input object
     *
     * @param window Window instance from which key presses are retrieved from
     */
    TerminalInput(WINDOW* window);

    /**
     * @brief Destroys the Terminal Input object
     *
     */
    ~TerminalInput();

        /**
     * @brief Method used to determine whether or not a current input button is currently pressed
     *
     * @param input_code Hex code for one of the 16 inputs to the Chip-8 emulator
     *
     * @return True if the key is pressed, false if not
     */
    virtual bool isPressed(uint8_t input_code);

    /**
     * @brief Blocks until one of the 16 valid keys is pressd
     *
     * @return The key pressed
     */
    virtual uint8_t getInput();

    /**
     * @brief Method used to update the input state map
     *
     */
    virtual void updateInputState();

private:

    /**
     * @brief Instance of the window input is retrieved from
     *
     */
    WINDOW* window_;

    /**
     * @brief Last character retrieved from input
     *
     */
    uint8_t last_keypress;

    /**
     * @brief Mutext to control thread access
     *
     */
    mutex read_input_mutext_;


    mutex input_stale_mutex;

    mutex lock_mutex;

    /**
     * @brief A flag used to indicate whether or not the last input char has already been read
     *
     */
    bool input_stale = false;


    /**
     * @brief Thread used to continuously update the input state
     *
     */
    thread* input_thread_;
};

#endif