#pragma once

#include <vector>

#include "utils/AST.hpp"
#include "lex.hpp"
#include "err.hpp"

//helper functions

//optional
//one or many
//alt

//main parse functions
AST_Node parse_program(std::vector<Token> &input);