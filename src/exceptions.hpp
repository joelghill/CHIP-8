#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include<exception>
#include<string>

using namespace std;

/**
 * @brief Exception thrown if in op code has not been implemented by the emulator
 *
 */
class OperationNotImplementedException : public exception {

public:

    // Construct with given error message:
    OperationNotImplementedException(string error = "Functionality not yet implemented!");

    // Provided for compatibility with std::exception.
    const char * what() const noexcept;

private:

     std::string errorMessage;
};

/**
 * @brief Exception throw if an invalid push or pop is performed on the stack
 *
 */
class InvalidStackOperationException : public exception {

public:

    // Construct with given error message:
    InvalidStackOperationException(string error = "That operation cannot be performed on the stack");

    // Provided for compatibility with std::exception.
    const char * what() const noexcept;

private:

     std::string errorMessage;
};

#endif