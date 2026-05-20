#include "parse.hpp"

using namespace std;

//helper function prototypes
AST_Nib_Pair_t alt_types(std::vector<TOK_TYPE> types, Nibbler nibbler);
AST_Nib_Pair_t alt(std::vector<std::function< AST_Nib_Pair_t(Nibbler) >> funcs, Nibbler nibbler);
AST_Nib_Pair_t require(Nibbler nibbler, TOK_TYPE type); //creates an AST_Node with type NON on success, throws ScribbleErr if not
Nibbler opt(Nibbler nibbler, std::function< void(Nibbler) > func);

//useful variables
static AST_Node _;

//parser functions

// VARTYPE , identifier , {',' , identifier} , ['=' , expression]
AST_Nib_Pair_t parse_variable_def(Nibbler nibbler) {
    AST_Node t(NODE_TYPE::VAR_TYPE);
    vector<AST_Node> idents(1);
    AST_Node expr;

    tie(t, nibbler) = alt_types({TOK_TYPE::STRING_TYPE, TOK_TYPE::NUMBER_TYPE, TOK_TYPE::FLOAT_TYPE}, nibbler);
    tie(idents[0], nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);

    while(true) {
        try {
            AST_Node tmp, _;
            tie(_, nibbler) = require(nibbler, TOK_TYPE::COMMA);
            tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
            idents.push_back(tmp);
        } catch(ScribbleErr&) {}
    }

    nibbler = opt(nibbler, [&] (Nibbler nibbler) {
        tie(_, nibbler) = require(nibbler, TOK_TYPE::EQUALS);
        //tie(expr, nibbler) = parse_expression()
        return nibbler;
    });

    //construct final node
    AST_Node res(NODE_TYPE::VARIABLE_DEF);
    res.children.push_back(t);
    for(auto& ident : idents)
        res.children.push_back(ident);

    return {res, nibbler};
}

AST_Nib_Pair_t parse_expression(Nibbler nibbler) {

}

//define helper functions
AST_Nib_Pair_t alt_types(std::vector<TOK_TYPE> types, Nibbler nibbler) {
    AST_Node node_res(NODE_TYPE::NON);
    ScribbleErr last_e = {0, 0, "", ERR_TYPE::EXPECTED};

    for(TOK_TYPE &type : types) {
        try {
            tie(node_res, nibbler) = require(nibbler, type);
            return {node_res, nibbler};
        } catch(ScribbleErr &e) { last_e = e; }
    }

    throw last_e;
    return {node_res, nibbler};
}

AST_Nib_Pair_t alt(std::vector<std::function< AST_Nib_Pair_t(Nibbler) >> funcs, Nibbler nibbler) {
    AST_Node node_res(NODE_TYPE::NON);
    ScribbleErr last_e = {0, 0, "", ERR_TYPE::EXPECTED};

    //loop through each function to try to find a pattern match
    for(auto func : funcs) {
        try {
            tie(node_res, nibbler) = func(nibbler); //if this returns without throwing an error a pattern was found
            return {node_res, nibbler};
        } catch(ScribbleErr &e) { last_e = e; }
    }
    
    //if no pattern was found throw the last error reaped from scanning
    throw last_e;
    return {node_res, nibbler};
}

AST_Nib_Pair_t require(Nibbler nibbler, TOK_TYPE type) {
    AST_Node tmp(NODE_TYPE::NON);
    Token next = nibbler.next();
    if(next.type == type) {
        tmp.tok = next;
        return {tmp, nibbler};
    }

    throw (ScribbleErr) { next.line, next.start_col, "", ERR_TYPE::EXPECTED };
    return {tmp, nibbler};
}

Nibbler opt(Nibbler nibbler, std::function< void(Nibbler) > func) {
    try {
        Nibbler n_copy = nibbler;
        func(n_copy);
        return n_copy; //return new position after successful run
    } catch(ScribbleErr&) {}

    return nibbler; //return original location
}

//define the Nibbler helper class
tok_vec_t* Nibbler::tokens = nullptr;

Nibbler::Nibbler(tok_vec_t* tokens, size_t pos) {
    Nibbler::tokens = tokens;
    this->pos = pos;
}

Nibbler::Nibbler(size_t pos) {
    this->pos = pos;
}

Token Nibbler::next() {
    if(pos >= tokens->size()) throw (ScribbleErr) {0, 0, "", ERR_TYPE::EOT};
    return tokens->at(pos++);
}