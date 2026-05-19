#include <iostream>
#include "utils/file_reader.hpp"

using namespace std;

file_reader::file_reader::file_reader(string* source) {
    this->source = source;
    it = source->begin();
    loc = {0,0};
}

/* return true if the source string is exhausted */
bool file_reader::file_reader::empty() {
    return it >= source->end();
}

/* Get the next item in the source string without consuming */
char file_reader::file_reader::peek() {
    if(it >= source->end()) throw (read_err) {0};

    return *it;
}

/* Get the next item in the source string and consume it */
char file_reader::file_reader::next() {
    if(it >= source->end()) throw (read_err) {0};
    char next = *(it++);
    loc.col++;

    //check if we need to set the position
    if(next == '\n') {
        loc.col = 0;
        loc.row++;
    }

    return next;
}

/* Check if the source string has a pattern in the current read position */
bool file_reader::file_reader::has_next(string pattern, bool strict, bool consume) {
    string::iterator tit = it; //make a temporary iterator (t-it)

    for(size_t i=0; i<pattern.size(); i++) {
        if(tit >= source->end()) return false;
        if(*(tit++) != pattern[i]) return false;
    }

    if(strict && tit < source->end()) {
        if(isalpha(*tit)) return false;
    }

    if(consume) it = tit;

    return true;
}

bool file_reader::file_reader::has_next(std::string pattern, std::string &lexeme, bool strict) {
    string::iterator tit = it; //make a temporary iterator (t-it)

    for(size_t i=0; i<pattern.size(); i++) {
        if(tit >= source->end()) return false;
        if(*(tit++) != pattern[i]) return false;
    }

    if(strict && tit < source->end()) {
        if(isalpha(*tit)) return false;
    }

    it = tit; //consume since we found the pattern
    lexeme += pattern;

    return true;
}