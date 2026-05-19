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
    TOK_TYPE next(); //throws ScribbleError when encountering an EOF
    size_t pos;
private:
    static tok_vec_t* tokens;
};

//helper functions
AST_Nib_Pair_t alt_types(std::vector<TOK_TYPE> types, Nibbler nibbler);
AST_Nib_Pair_t alt(std::vector<std::function< AST_Nib_Pair_t(Nibbler) >> funcs, Nibbler nibbler);
AST_Nib_Pair_t require(Nibbler nibbler, TOK_TYPE type);

//main parse functions
AST_Nib_Pair_t parse_program(Nibbler nibbler);

AST_Nib_Pair_t parse_variable_dev(Nibbler nibbler);