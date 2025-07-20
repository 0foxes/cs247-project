#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
// #include <utility>

enum ErrorType { NONE, INVALID_PARAMETERS, INVALID_OBJECT_INSTANCE };
class Error {
    ErrorType errortype;
    std::string errorstring;

  public:
    // the default error is no error
    Error(ErrorType type = ErrorType::NONE,
          std::string msg = "No further details");
    // to print it
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& out, const Error& error);

#endif