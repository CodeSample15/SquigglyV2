#pragma once

#include <vector>
#include <string>

enum TOK_TYPE {
    //preprocessor
    USE,                            // use
    AS,                             // as

    //keywords
    IF,                             // if
    IF_ELSE,                        // if else
    ELSE,                           // else

    FUNCTION_DEFINE,                // fun

    //built in types
    NUMBER,                         // num
    FLOAT,                          // float
    STRING,                         // string

    //values
    IDENTIFIER,                     // any string of characters
    STRING_LITERAL,                 // "{content}" or '{content}'
    INT_LITERAL,                    // any int
    FLOAT_LITERAL,                  // any float

    //symbols
    OPEN_PAREN, CLOSE_PAREN,        // ()
    OPEN_CURLY, CLOSE_CURLY,        // {}
    OPEN_BRACKET, CLOSE_BRACKET,    // []

    SEMICOLON,                      // ;
    COMMA,                          // ,
    PLUS,                           // +
    MINUS,                          // -
    SLASH,                          // /
    STAR,                           // *
    BAR,                            // |
    UP_ARROW,                       // ^
    GREATER_THAN,                   // >
    LESS_THAN,                      // <
    SHIFT_RIGHT,                    // >>
    SHIFT_LEFT,                     // <<

    EQUALS,                         // =
    PLUS_EQUALS,                    // +=
    MINUS_EQUALS,                   // -=
    SLASH_EQUALS,                   // /=
    STAR_EQUALS,                    // *=

    INCR,                           // ++
    DECR,                           // --

    //special characters
    IMAGE_REF,                      // @
    BUILT_IN_VARIABLE_REF,          // $
    SPECIAL_FUNCTION_PREFIX,        // :

    //booleans
    AND,                            // &&
    OR,                             // ||
    NOT,                            // !
    
    //ignored (stripped from lexer output)
    WHITESPACE,
    COMMENT
};

struct Token {
    size_t line;
    size_t start_col;
    size_t end_col;
    std::string lexeme;
    TOK_TYPE type;
};

/*
    Lex the program and return the tokens which represent the raw source lines fed into the program.

    Throws a ScribbleErr (err.hpp), with a type of error 
*/
std::vector<Token> lex(std::string &source);