#pragma once

#include <vector>
#include <functional>
#include <utility>

#include "utils/AST.hpp"
#include "lex.hpp"
#include "err.hpp"

//useful types
#define AST_Nib_Pair_t std::pair<AST_Node, Nibbler>
typedef std::vector<Token> tok_vec_t;

//helper class to consume tokens. Called nibbler cuz I'm taking small nibbles from the token vector
class Nibbler {
public:
    Nibbler(tok_vec_t* tokens, size_t pos=0);
    Nibbler(size_t pos=0);
    Token next(); //throws ScribbleError when encountering an EOF
    size_t pos;
private:
    static tok_vec_t* tokens;
};

//main parse functions
AST_Nib_Pair_t parse_program(Nibbler nibbler);
AST_Nib_Pair_t parse_variable_def(Nibbler nibbler);
AST_Nib_Pair_t parse_expression(Nibbler nibbler);