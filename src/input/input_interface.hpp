/**
 * @file input_interface.hpp
 * @author Joel Hill (joel.hill.87@gmail.com)
 * @brief
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef INPUT_INTERFACE_H
#define INPUT_INTERFACE_H

#include <iostream>

using namespace std;

/**
 * @brief Interface for input into the Chip-8 emulator
 *
 */
class InputInterface
{
public:
    /**
     * @brief Destroy the Input Interface object
     *
     */
    virtual ~InputInterface() = default;

    /**
     * @brief Method used to determine whether or not a current input button is currently pressed
     *
     * @param input_code Hex code for one of the 16 inputs to the Chip-8 emulator
     * @return true If the provided input is currently pressed
     * @return false If the provided input is not currently pressed
     */
    virtual bool isPressed(uint8_t input_code) = 0;

    /**
     * @brief Blocks until one of the 16 valid keys is pressd
     *
     * @return uint8_t A value in the range of 0x0 to 0xF
     */
    virtual uint8_t getInput() = 0;
};

#endif