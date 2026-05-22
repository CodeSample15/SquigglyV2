#include "parse.hpp"

using namespace std;

//helper function prototypes
AST_Nib_Pair_t alt_types(std::vector<TOK_TYPE> types, Nibbler nibbler);
AST_Nib_Pair_t alt(std::vector<std::function< AST_Nib_Pair_t(Nibbler) >> funcs, Nibbler nibbler);
AST_Nib_Pair_t require(Nibbler nibbler, TOK_TYPE type); //creates an AST_Node with type NON on success, throws ScribbleErr if not
Nibbler opt(Nibbler nibbler, std::function< Nibbler(Nibbler) > func);
Nibbler many_0(Nibbler nibbler, std::function< Nibbler(Nibbler) > func);

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

    nibbler = many_0(nibbler, [&](Nibbler n){
        AST_Node tmp;
        tie(_, nibbler) = require(nibbler, TOK_TYPE::COMMA);
        tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
        idents.push_back(tmp);
        return n;
    });

    nibbler = opt(nibbler, [&] (Nibbler n) {
        tie(_, n) = require(n, TOK_TYPE::EQUALS);
        //tie(expr, nibbler) = parse_expression()
        return n;
    });

    //construct final node
    AST_Node res(NODE_TYPE::VARIABLE_DEF);
    res.children.push_back(t);
    for(auto& ident : idents)
        res.children.push_back(ident);

    return {res, nibbler};
}

//['$'] , identifier , ['[' , expression , {',' expression} ']']
AST_Nib_Pair_t parse_variable_reference(Nibbler nibbler) {

}

//exp_andl , { '||' , exp_andl }
AST_Nib_Pair_t parse_expression(Nibbler nibbler) {
    return {_, nibbler};
}

//exp_or , { '&' , exp_or }
AST_Nib_Pair_t parse_exp_andl(Nibbler nibbler) {

}

//exp_xor , { '|' , exp_xor }
AST_Nib_Pair_t parse_exp_or(Nibbler nibbler) {

}

//exp_and , { '^' , exp_and }
AST_Nib_Pair_t parse_exp_xor(Nibbler nibbler) {

}

//exp_eq , { '&' , exp_and }
AST_Nib_Pair_t parse_exp_and(Nibbler nibbler) {

}

//exp_cmp , { ('==' | '!=') , exp_cmp }
AST_Nib_Pair_t parse_exp_eq(Nibbler nibbler) {

}

//exp_shft , { ('<' | '<=' | '>' | '>=') , exp_shft }
AST_Nib_Pair_t parse_exp_cmp(Nibbler nibbler) {

}

//exp_add , { ('<<' , '>>') , exp_add }
AST_Nib_Pair_t parse_exp_shft(Nibbler nibbler) {

}

//exp_mult , { ('+' | '-') , exp_mult }
AST_Nib_Pair_t parse_exp_add(Nibbler nibbler) {

}

//exp_pow , { ('*' | '/' | '%') , exp_pow }
AST_Nib_Pair_t parse_exp_mult(Nibbler nibbler) {

}

//exp_not , [ '**' , exp_pow ]
AST_Nib_Pair_t parse_exp_pow(Nibbler nibbler) {

}

//['!'] , exp_primary
AST_Nib_Pair_t parse_exp_not(Nibbler nibbler) {

}

//variable_reference | literal | '(' , expression , ')' | function_call
AST_Nib_Pair_t parse_exp_primary(Nibbler nibbler) {
    
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
    for(auto &func : funcs) {
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

Nibbler opt(Nibbler nibbler, std::function< Nibbler(Nibbler) > func) {
    try {
        return func(nibbler); //return new location
    } catch(ScribbleErr&) {}

    return nibbler; //return original location
}

Nibbler many_0(Nibbler nibbler, std::function< Nibbler(Nibbler) > func) {
    while(true) {
        try {
            nibbler = func(nibbler);
        } catch(ScribbleErr&) { break; }
    }

    return nibbler;
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