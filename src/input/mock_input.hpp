#ifndef MOCK_INTERFACE_H
#define MOCK_INTERFACE_H

#include <iostream>
#include "input_interface.hpp"

using namespace std;

/**
 * @brief A mock implementation of an input device
 *
 */
class MockInput : public InputInterface
{

public:
    /**
     * @brief Constructs a new Mock Input object
     *
     */
    MockInput() = default;

        /**
     * @brief Method used to determine whether or not a current input button is currently pressed
     *
     * @param input_code Hex code for one of the 16 inputs to the Chip-8 emulator
     * @return true If 1 is randomly chosen
     * @return false If 0 is randomly chosen
     */
    virtual bool isPressed(uint8_t input_code);

    /**
     * @brief Blocks until one of the 16 valid keys is pressd
     *
     * @return uint8_t A random value in the range of 0x0 to 0xF
     */
    virtual uint8_t getInput();

};

#endif