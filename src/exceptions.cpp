#include<exception>
#include<string>

using namespace std;

class OperationNotImplementedException : public exception {

public:

    // Construct with given error message:
    OperationNotImplementedException(string error = "Functionality not yet implemented!") {
        errorMessage = error;
    }

    // Provided for compatibility with std::exception.
    const char * what() const noexcept
    {
        return errorMessage.c_str();
    }

private:

     std::string errorMessage;
};