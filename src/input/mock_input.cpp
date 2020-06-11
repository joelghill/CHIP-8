#include <iostream>
#include "mock_input.hpp"

using namespace std;


bool MockInput::isPressed(uint8_t input_code) {
    return (bool)rand() % 2;
}

uint8_t MockInput::getInput() {
    return (uint8_t)rand() % 9;
}