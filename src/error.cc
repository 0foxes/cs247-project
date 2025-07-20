#include "error.h"

Error::Error(ErrorType type, std::string msg)
    : errortype{type}, errorstring{msg} {
    // later: if there is no error but there is an error message, get angy?
}

std::string Error::toString() const {
    switch (errortype) {
    case ErrorType::NONE:
        return "No error";
    case ErrorType::INVALID_PARAMETERS:
        return "Invalid Parameters: " + errorstring;
    case ErrorType::INVALID_OBJECT_INSTANCE:
        return "Invalid Object Instance: " + errorstring;
    default:
        return "How did you do this.";
    }
}

std::ostream& operator<<(std::ostream& out, const Error& error) {
    return out << error.toString() << std::endl;
}