#pragma once

/*
    Basic utility class to manage reading from a source string without causing OOB.

    The file_reader class functions throw a read_err when an OOB occurs. This can be caught by calling
    functions to easily stop the parsing process. At least that's the goal here
*/

#include <string>

namespace file_reader {

struct read_err {
    bool fatal;
};

struct src_loc {
    size_t row;
    size_t col;
};

/*
    Read content from an input string without causing OOB errors
*/
class file_reader {
public:
    file_reader(std::string* source);

    /* return true if the source string is exhausted */
    bool empty();

    /* Get the next item in the source string without consuming */
    char peek();

    /* Get the next item in the source string and consume it */
    char next();

    /* Check if the source string has a pattern in the current read position. If strict, no other letters can exist immediately after the pattern */
    bool has_next(std::string pattern, bool strict=true, bool consume=true);
    bool has_next(std::string pattern, std::string &lexeme, bool strict=true);
    
    src_loc loc;

private:
    std::string* source;
    std::string::iterator it;
};

}