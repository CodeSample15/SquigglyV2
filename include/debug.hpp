#pragma once

#include "lex.hpp"

/*
    Functions which will be used to debug the Scribble interpreter
*/

void print_tokens(std::vector<Token> &tokens);
void print_AST();