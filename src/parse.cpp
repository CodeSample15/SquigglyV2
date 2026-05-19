#include "parse.hpp"

using namespace std;

//parser functions

// VARTYPE , identifier , {',' , identifier} , ['=' , expression]
AST_Nib_Pair_t parse_variable_dev(Nibbler nibbler) {
    AST_Node t(NODE_TYPE::VAR_TYPE);
    AST_Node ident(NODE_TYPE::IDENTIFIER);

    tie(t, nibbler) = alt_types({TOK_TYPE::STRING_TYPE, TOK_TYPE::NUMBER_TYPE, TOK_TYPE::FLOAT_TYPE}, nibbler);
    tie(ident, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
}

//helper functions


//define the Nibbler helper class
Nibbler::Nibbler(tok_vec_t* tokens, size_t pos) {
    this->tokens = tokens;
    this->pos = pos;
}

Nibbler::Nibbler(size_t pos) {
    this->pos = pos;
}

TOK_TYPE Nibbler::next() {
    if(pos >= tokens->size()) throw (ScribbleErr) {0, 0, "", ERR_TYPE::EOT};
    return tokens->at(pos++).type;
}