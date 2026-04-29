#pragma once

#include <string>

typedef enum {
    // general errors
    UNIMPLEMENTED,

    // lexer errors
    UNKNOWN_CHARACTER,

    // parser errors

    // runtime errors
} ERR_TYPE;

/*
    This struct can get thrown by the lexer, parser, or runner to signal an unrecoverable error
    was encountered by the program.
    Line, Col, Type 
*/
struct ScribbleErr {
    size_t line;
    size_t col;
    ERR_TYPE type;
};

void PrintSErrMessage(ScribbleErr err);