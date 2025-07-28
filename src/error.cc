#include "../includes/error.h"

Error::Error(ErrorType type, string msg) : errortype{type}, errorstring{msg} {
    // later: if there is no error but there is an error message, get angy?
}

string Error::toString() const {
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

ostream& operator<<(ostream& out, const Error& error) {
    return out << error.toString() << endl;
}