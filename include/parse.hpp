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

//variable related
AST_Nib_Pair_t parse_vartype(Nibbler nibbler);
AST_Nib_Pair_t parse_variable_def(Nibbler nibbler);
AST_Nib_Pair_t parse_variable_reference(Nibbler nibbler);
AST_Nib_Pair_t parse_vartype(Nibbler nibbler);

AST_Nib_Pair_t parse_arr_index(Nibbler nibbler);

//function related
AST_Nib_Pair_t parse_function_call(Nibbler nibbler);
AST_Nib_Pair_t parse_arguments(Nibbler nibbler);

//expressions
AST_Nib_Pair_t parse_expression(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_orl(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_andl(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_or(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_xor(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_and(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_eq(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_cmp(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_shft(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_add(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_mult(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_pow(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_not(Nibbler nibbler);
AST_Nib_Pair_t parse_exp_primary(Nibbler nibbler);