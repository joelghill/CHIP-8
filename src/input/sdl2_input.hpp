#ifndef TERMINAL_INPUT_H
#define TERMINAL_INPUT_H

#include <iostream>
#include <map>
#include <mutex>
#include <SDL2/SDL.h>
#include <thread>
#include "input_interface.hpp"

using namespace std;

/**
 * @brief An implementation of the InputInterface that uses the terminal for keyboard input
 *
 */
class SDL2Input : public InputInterface
{

public:

    /**
     * @brief Construct a new Terminal Input object
     *
     */
    SDL2Input();

    /**
     * @brief Destroys the Terminal Input object
     *
     */
    ~SDL2Input();

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

private:

    /**
     * @brief Last character retrieved from input
     *
     */
    map<SDL_Scancode, uint8_t> keymap_ = {

    };

    /**
     * @brief Thread used to continuously update the input state
     *
     */
    thread* input_thread_;

    /**
     * @brief
     *
     */
    SDL_Window *window_;
};

#endif