#include <iostream>
#include "parse.hpp"
#include "debug.hpp"

using namespace std;

//helper function prototypes
AST_Nib_Pair_t alt_types(std::vector<TOK_TYPE> types, Nibbler nibbler);
AST_Nib_Pair_t alt(std::vector<std::function< AST_Nib_Pair_t(Nibbler) >> funcs, Nibbler nibbler);
AST_Nib_Pair_t require(Nibbler nibbler, TOK_TYPE type); //creates an AST_Node with type NON on success, throws ScribbleErr if not
Nibbler opt(Nibbler nibbler, std::function< Nibbler(Nibbler) > func);
Nibbler many_0(Nibbler nibbler, std::function< Nibbler(Nibbler) > func);

AST_Nib_Pair_t expression_seg_parse(Nibbler nibbler, std::function< AST_Nib_Pair_t(Nibbler) > expression_seg, vector<TOK_TYPE> exp_symbols, NODE_TYPE out_type);

//useful variables
static AST_Node _;

//parser functions

AST_Nib_Pair_t parse_vartype(Nibbler nibbler) {
    return alt_types({TOK_TYPE::STRING_TYPE, TOK_TYPE::NUMBER_TYPE, TOK_TYPE::FLOAT_TYPE}, nibbler);
}

// VARTYPE , identifier , {',' , identifier} , ['=' , expression]
AST_Nib_Pair_t parse_variable_def(Nibbler nibbler) {
    AST_Node t(NODE_TYPE::VAR_TYPE);
    vector<AST_Node> idents(1);
    AST_Node expr;

    tie(t, nibbler) = parse_vartype(nibbler);
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
    throw (ScribbleErr) {0, 0, "var_ref", ERR_TYPE::UNIMPLEMENTED};
    return {_, nibbler};
}

//functions
AST_Nib_Pair_t parse_function_call(Nibbler nibbler) {
    throw (ScribbleErr) {0, 0, "function_call", ERR_TYPE::UNIMPLEMENTED};
    return {_, nibbler};
}

AST_Nib_Pair_t parse_arguments(Nibbler nibbler) {
    throw (ScribbleErr) {0, 0, "arguments", ERR_TYPE::UNIMPLEMENTED};
    return {_, nibbler};
}

//EXPRESSIONS

//exp_andl , { '||' , exp_andl }
AST_Nib_Pair_t parse_expression(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_andl, {TOK_TYPE::OR}, NODE_TYPE::EXP_ORL);
}

//exp_or , { '&&' , exp_or }
AST_Nib_Pair_t parse_exp_andl(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_or, {TOK_TYPE::AND}, NODE_TYPE::EXP_ANDL);
}

//exp_xor , { '|' , exp_xor }
AST_Nib_Pair_t parse_exp_or(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_xor, {TOK_TYPE::BAR}, NODE_TYPE::EXP_OR);
}

//exp_and , { '^' , exp_and }
AST_Nib_Pair_t parse_exp_xor(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_and, {TOK_TYPE::UP_ARROW}, NODE_TYPE::EXP_XOR);
}

//exp_eq , { '&' , exp_eq }
AST_Nib_Pair_t parse_exp_and(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_eq, {TOK_TYPE::BIT_AND}, NODE_TYPE::EXP_AND);
}

//exp_cmp , { ('==' | '!=') , exp_cmp }
AST_Nib_Pair_t parse_exp_eq(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_cmp, {TOK_TYPE::CMP_EQUALS, TOK_TYPE::CMP_NOT_EQUALS}, NODE_TYPE::EXP_EQ);
}

//exp_shft , { ('<' | '<=' | '>' | '>=') , exp_shft }
AST_Nib_Pair_t parse_exp_cmp(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_shft, {TOK_TYPE::LESS_THAN, TOK_TYPE::LESS_THAN_EQUAL, TOK_TYPE::GREATER_THAN, TOK_TYPE::GREATER_THAN_EQUAL}, NODE_TYPE::EXP_CMP);
}

//exp_add , { ('<<' , '>>') , exp_add }
AST_Nib_Pair_t parse_exp_shft(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_add, {TOK_TYPE::SHIFT_LEFT, TOK_TYPE::SHIFT_RIGHT}, NODE_TYPE::EXP_SHFT);
}

//exp_mult , { ('+' | '-') , exp_mult }
AST_Nib_Pair_t parse_exp_add(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_mult, {TOK_TYPE::PLUS, TOK_TYPE::MINUS}, NODE_TYPE::EXP_ADD);
}

//exp_pow , { ('*' | '/' | '%') , exp_pow }
AST_Nib_Pair_t parse_exp_mult(Nibbler nibbler) {
    return expression_seg_parse(nibbler, parse_exp_pow, {TOK_TYPE::STAR, TOK_TYPE::SLASH, TOK_TYPE::PERCENT}, NODE_TYPE::EXP_MULT);
}

//exp_not , [ '**' , exp_pow ]
AST_Nib_Pair_t parse_exp_pow(Nibbler nibbler) {
    AST_Node not_node;
    tie(not_node, nibbler) = parse_exp_not(nibbler);

    AST_Node tmp;
    bool use_pow = false;
    try {
        tie(_, nibbler) = require(nibbler, TOK_TYPE::POW);
        if(_.type==NODE_TYPE::NON) //this will run no matter what it's just to please the compiler (it thinks this is an infinite recursion case but it really isn't I swear)
            tie(tmp, nibbler) = parse_exp_pow(nibbler);

        use_pow = true;
    } catch(ScribbleErr&) {}

    if(use_pow) {
        AST_Node pow_node;
        pow_node.type = NODE_TYPE::EXP_POW;
        pow_node.children.push_back(not_node);
        pow_node.children.push_back(tmp);
        return {pow_node, nibbler};
    }

    return {not_node, nibbler};
}

//['!'] , exp_primary
AST_Nib_Pair_t parse_exp_not(Nibbler nibbler) {
    AST_Node not_node;
    bool use_not = false;
    try {
        tie(not_node, nibbler) = require(nibbler, TOK_TYPE::NOT);
        use_not = true;
    } catch(ScribbleErr&) {}

    AST_Node primary;
    tie(primary, nibbler) = parse_exp_primary(nibbler);

    if(use_not) {
        not_node.children.push_back(primary);
        return {not_node, nibbler};
    }

    return {primary, nibbler};
}

//variable_reference | literal | '(' , expression , ')' | function_call
AST_Nib_Pair_t parse_exp_primary(Nibbler nibbler) {
    return alt({parse_variable_reference, 
                parse_function_call,
                [&](Nibbler n){ return alt_types({TOK_TYPE::STRING_LITERAL, TOK_TYPE::INT_LITERAL, TOK_TYPE::FLOAT_LITERAL}, nibbler); },
                [&](Nibbler n){
                    AST_Node expr;
                    n = require(n, TOK_TYPE::OPEN_PAREN).second;
                    tie(expr, n) = parse_expression(n);
                    n = require(n, TOK_TYPE::CLOSE_PAREN).second;

                    return (AST_Nib_Pair_t){expr, n};
                }}, nibbler);
}

//helper function 
AST_Nib_Pair_t expression_seg_parse(Nibbler nibbler, std::function< AST_Nib_Pair_t(Nibbler) > expression_seg, vector<TOK_TYPE> exp_symbols, NODE_TYPE out_type) {
    AST_Node first;
    tie(first, nibbler) = expression_seg(nibbler);

    nibbler = many_0(nibbler, [&](Nibbler n) {
        AST_Node symbol;
        tie(symbol, n) = alt_types(exp_symbols, n);

        AST_Node second;
        tie(second, n) = expression_seg(n);
        
        symbol.children.push_back(first);
        symbol.children.push_back(second);
        symbol.type = out_type;
        first = symbol; //return this symbol to join the two segments

        return n;
    });

    return {first, nibbler};
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
        //set tmp node type depending on the token type
        switch(type) {
            case TOK_TYPE::STRING_LITERAL:
            case TOK_TYPE::INT_LITERAL:
            case TOK_TYPE::FLOAT_LITERAL:
            tmp.type = NODE_TYPE::EXP_PRIMARY;
            break;

            default: break;
        }

        tmp.tok = next;
        return {tmp, nibbler};
    }

    throw (ScribbleErr) { next.line, next.start_col, tok_type_to_string(type), ERR_TYPE::EXPECTED };
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