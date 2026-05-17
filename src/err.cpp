#include <iostream>
#include "err.hpp"

using namespace std;

std::string errmsg(ERR_TYPE t);

void PrintSErrMessage(ScribbleErr e) {
    cout << "---------------------" << endl;
    cout << "Error at (line, col) - " << e.line << ", " << e.col << ":" << endl;
    cout << errmsg(e.type) << endl;
    cout << "---------------------" << endl;
}

std::string errmsg(ERR_TYPE t) {
    switch(t) {
        case ERR_TYPE::UNIMPLEMENTED:
            return "NOT IMPLEMENTED";
        case ERR_TYPE::UNKNOWN_CHARACTER:
            return "Uknown character in source";
        case ERR_TYPE::UNCLOSED_QUOTE:
            return "Expected closing quote";
        case ERR_TYPE::UNRECOGNIZED_PATTERN:
            return "Unrecognized pattern in source code";
        default:
            return "Unimplemented Error Message";
    }
}