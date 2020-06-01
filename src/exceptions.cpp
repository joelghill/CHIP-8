#include<exception>
#include<string>
#include "exceptions.hpp"

using namespace std;

OperationNotImplementedException::OperationNotImplementedException(string error) {
    errorMessage = error;
}

const char * OperationNotImplementedException::what() const noexcept
{
    return errorMessage.c_str();
}


// Construct with given error message:
InvalidStackOperationException::InvalidStackOperationException(string error) {
    errorMessage = error;
}

// Provided for compatibility with std::exception.
const char* InvalidStackOperationException::what() const noexcept {
    return errorMessage.c_str();
}