#ifndef INPUT_FACTORY_H
#define INPUT_FACTORY_H

#include <iostream>
#include "input_interface.hpp"

using namespace std;

/**
 * @brief Enumeration defining various input types
 *
 */
enum class InputPlatform {
    WINDOWS,
    OSX,
    LINUX,
    ANDROID
};

/**
 * @brief Class used to generate Input instances
 *
 */
class InputFactory
{

public:
    /**
     * @brief Get an Input based on the desired input platform
     *
     * @param platform The platform to construct an input instance for
     * @return InputInterface* instance
     */
    static InputInterface* GetInputInstance(InputPlatform platform);

    /**
     * @brief Get the Mock Input object
     *
     * @return InputInterface* A mock implementation of an input device
     */
    static InputInterface* GetMockInput();
};

#endif