#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
// #include <utility>

using namespace std;

enum ErrorType { NONE, INVALID_PARAMETERS, INVALID_OBJECT_INSTANCE };
class Error {
    ErrorType errortype;
    string errorstring;

  public:
    // the default error is no error
    Error(ErrorType type = ErrorType::NONE, string msg = "No further details");
    // to print it
    string toString() const;
};

ostream& operator<<(ostream& out, const Error& error);

#endif