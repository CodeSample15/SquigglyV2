#pragma once

#include "lex.hpp"

/*
    Functions which will be used to debug the Scribble interpreter
*/

std::string tok_type_to_string(TOK_TYPE tok);

void print_tokens(std::vector<Token> &tokens);
void print_AST();