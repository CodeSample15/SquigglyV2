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

//preprocessor

//main parse functions

//start_func | update_func
AST_Nib_Pair_t parse_core_function(Nibbler nibbler) {
    return alt({parse_start_func, parse_update_func}, nibbler);
}

//':START:{' , body , '}'
AST_Nib_Pair_t parse_start_func(Nibbler nibbler) {
    AST_Node start_node(NODE_TYPE::START_FUNC);
    AST_Node tmp;

    //header
    nibbler = require(nibbler, TOK_TYPE::SPECIAL_FUNCTION_PREFIX).second;
    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    if(tmp.tok.lexeme != "START") throw (ScribbleErr) { tmp.tok.line, tmp.tok.start_col, "START", ERR_TYPE::EXPECTED };
    nibbler = require(nibbler, TOK_TYPE::SPECIAL_FUNCTION_PREFIX).second;
    
    //body
    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    start_node.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {start_node, nibbler};
}

//':UPDATE:{' , body , '}'
AST_Nib_Pair_t parse_update_func(Nibbler nibbler) {
    AST_Node update_node(NODE_TYPE::UPDATE_FUNC);
    AST_Node tmp;

    //header
    nibbler = require(nibbler, TOK_TYPE::SPECIAL_FUNCTION_PREFIX).second;
    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    if(tmp.tok.lexeme != "UPDATE") throw (ScribbleErr) { tmp.tok.line, tmp.tok.start_col, "UPDATE", ERR_TYPE::EXPECTED };
    nibbler = require(nibbler, TOK_TYPE::SPECIAL_FUNCTION_PREFIX).second;
    
    //body
    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    update_node.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {update_node, nibbler};
}

//parser functions

AST_Nib_Pair_t parse_vartype(Nibbler nibbler) {
    AST_Node tmp;
    tie(tmp, nibbler) = alt_types({TOK_TYPE::STRING_TYPE, TOK_TYPE::NUMBER_TYPE, TOK_TYPE::FLOAT_TYPE}, nibbler);
    tmp.type = NODE_TYPE::VAR_TYPE;
    
    return {tmp, nibbler};
}

// VARTYPE , identifier , {',' , identifier} , ['=' , expression]
AST_Nib_Pair_t parse_variable_def(Nibbler nibbler) {
    AST_Node t;
    vector<AST_Node> idents(1);
    AST_Node expr; 
    bool add_expr = false;

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
        tie(expr, n) = parse_expression(n);
        add_expr = true;
        return n;
    });

    //construct final node
    AST_Node res(NODE_TYPE::VARIABLE_DEF);
    t.type = NODE_TYPE::VAR_TYPE;
    res.children.push_back(t);
    for(auto& ident : idents) {
        ident.type = NODE_TYPE::IDENT;
        res.children.push_back(ident);
    }

    if(add_expr)
        res.children.push_back(expr);

    return {res, nibbler};
}

//['$'] , identifier , ['[' , arguments , ']']
AST_Nib_Pair_t parse_variable_reference(Nibbler nibbler) {
    bool built_in = false;
    try {
        nibbler = require(nibbler, TOK_TYPE::BUILT_IN_VARIABLE_REF).second;
        built_in = true;
    } catch (ScribbleErr&) {}

    AST_Node var_name;
    tie(var_name, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);

    if(built_in)
        var_name.tok.lexeme = '$' + var_name.tok.lexeme;
    var_name.type = NODE_TYPE::IDENT;

    AST_Node arr_index;
    bool add_arr_index = false;
    opt(nibbler, [&](Nibbler n) {
        n = require(n, TOK_TYPE::OPEN_BRACKET).second;
        tie(arr_index, n) = parse_arguments(n);
        n = require(n, TOK_TYPE::CLOSE_BRACKET).second;

        add_arr_index = true;
        return n;
    });

    //create result and return
    AST_Node res(NODE_TYPE::VARIABLE_REFERENCE);
    res.tok = var_name.tok;
    res.children.push_back(var_name);
    if(add_arr_index)
        res.children.push_back(arr_index);

    return {res, nibbler};
}

//variable_reference , INCR_DECR_OP | (ASSIGN_OP , expression)
AST_Nib_Pair_t parse_variable_assign(Nibbler nibbler) {
    AST_Node var_ref;

    tie(var_ref, nibbler) = parse_variable_reference(nibbler);

    AST_Node assign_node;
    tie(assign_node, nibbler) = alt({
        [&](Nibbler n) {
            return alt_types({TOK_TYPE::INCR, TOK_TYPE::DECR}, n); 
        },
        [&](Nibbler n) {
            AST_Node tmp_a, tmp_e;
            tie(tmp_a, n) = alt_types({TOK_TYPE::EQUALS, TOK_TYPE::PLUS_EQUALS, TOK_TYPE::MINUS_EQUALS, TOK_TYPE::STAR_EQUALS, TOK_TYPE::SLASH_EQUALS}, n);
            tie(tmp_e, n) = parse_expression(n);

            tmp_a.children.push_back(tmp_e);
            return (AST_Nib_Pair_t) {tmp_a,n};
        }
    }, nibbler);

    assign_node.type = NODE_TYPE::ASSIGN;

    //compile and return result
    AST_Node res(NODE_TYPE::VARIABLE_ASSIGN);
    res.tok = var_ref.tok;
    res.children.push_back(var_ref);
    res.children.push_back(assign_node);

    return {res, nibbler};
}

//FUNCTIONS

//[function_modifier] , 'fun' , identifier , '(' , [parameters] , ')' , '{' , body , '}'
AST_Nib_Pair_t parse_function_def(Nibbler nibbler) {
    AST_Node function_def(NODE_TYPE::FUNCTION_DEF);
    AST_Node tmp;

    nibbler = opt(nibbler, [&](Nibbler n) {
        tie(tmp, n) = parse_function_modifier(n);
        function_def.children.push_back(tmp);
        return n;
    });

    nibbler = require(nibbler, TOK_TYPE::FUNCTION_DEFINE).second;

    //function identifier
    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    function_def.tok = tmp.tok;

    //get parameters
    nibbler = require(nibbler, TOK_TYPE::OPEN_PAREN).second;
    nibbler = opt(nibbler, [&](Nibbler n) {
        tie(tmp, n) = parse_parameters(n);
        function_def.children.push_back(tmp);
        return n;
    });
    nibbler = require(nibbler, TOK_TYPE::CLOSE_PAREN).second;

    //get function body
    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    function_def.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {function_def, nibbler};
}

//'[' , VALID_FUNCTION_MODIFIER , {',' , VALID_FUNCTION_MODIFIER} , ']'
AST_Nib_Pair_t parse_function_modifier(Nibbler nibbler) {
    AST_Node modifier_node(NODE_TYPE::FUNCTION_MODIFIER);
    AST_Node tmp;

    nibbler = require(nibbler, TOK_TYPE::OPEN_BRACKET).second;

    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    modifier_node.children.push_back(tmp);

    nibbler = many_0(nibbler, [&](Nibbler n) {
        n = require(n, TOK_TYPE::COMMA).second;
        tie(tmp, n) = require(n, TOK_TYPE::IDENTIFIER);
        modifier_node.children.push_back(tmp);

        return n;
    });

    return {modifier_node, nibbler};
}

//identifier , '(' , [arguments] , ')'
AST_Nib_Pair_t parse_function_call(Nibbler nibbler) {
    AST_Node ident;
    tie(ident, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    ident.type = NODE_TYPE::FUNCTION_CALL;

    AST_Node args;
    nibbler = require(nibbler, TOK_TYPE::OPEN_PAREN).second;
    nibbler = opt(nibbler, [&](Nibbler n) {
        tie(args, n) = parse_arguments(n);
        ident.children.push_back(args);
        return n;
    });
    nibbler = require(nibbler, TOK_TYPE::CLOSE_PAREN).second;

    return {ident, nibbler};
}

//VARTYPE , identifier , {',' , VARTYPE , identifier}
AST_Nib_Pair_t parse_parameters(Nibbler nibbler) {
    AST_Node arguments_node(NODE_TYPE::ARGUMENTS);
    AST_Node tmp;

    tie(tmp, nibbler) = parse_vartype(nibbler);
    arguments_node.children.push_back(tmp);

    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IDENTIFIER);
    arguments_node.children.push_back(tmp);

    nibbler = many_0(nibbler, [&](Nibbler n) {
        n = require(n, TOK_TYPE::COMMA).second;

        tie(tmp, n) = parse_vartype(n);
        arguments_node.children.push_back(tmp);

        tie(tmp, n) = require(n, TOK_TYPE::IDENTIFIER);
        arguments_node.children.push_back(tmp);

        return n;
    });
}

//expression , { ',' , expression }
AST_Nib_Pair_t parse_arguments(Nibbler nibbler) {
    AST_Node arr_index(NODE_TYPE::ARGUMENTS);
    AST_Node tmp;

    tie(tmp, nibbler) = parse_expression(nibbler);
    arr_index.children.push_back(tmp);
    
    nibbler = many_0(nibbler, [&](Nibbler n) {
        n = require(n, TOK_TYPE::COMMA).second;
        tie(tmp, n) = parse_expression(n);
        arr_index.children.push_back(tmp);

        return n;
    });

    return {arr_index, nibbler};
}

// { (variable_def | variable_assign | branch | function_call | loop) , [';'] }
AST_Nib_Pair_t parse_body(Nibbler nibbler) {
    AST_Node body(NODE_TYPE::BODY);
    AST_Node tmp;

    nibbler = many_0(nibbler, [&](Nibbler n) {
        tie(tmp, n) = alt({ parse_variable_def, parse_variable_assign, parse_branch, parse_function_call, parse_loop }, n);
        body.children.push_back(tmp);

        n = opt(n, [&](Nibbler n2) {
            n2 = require(n2, TOK_TYPE::SEMICOLON).second;
            return n2;
        });

        return n;
    });
    
    return {body, nibbler};
}

//BRANCHES

// branch = branch_if ,  {branch_ifelse} , [branch_else]
AST_Nib_Pair_t parse_branch(Nibbler nibbler) {
    AST_Node branch_node, tmp;

    branch_node.type = NODE_TYPE::BRANCH;

    tie(tmp, nibbler) = parse_branch_if(nibbler);
    branch_node.tok = tmp.tok;
    branch_node.children.push_back(tmp);

    nibbler = many_0(nibbler, [&](Nibbler n) {
        tie(tmp, n) = parse_branch_if_else(n);
        branch_node.children.push_back(tmp);
        return n;
    });

    nibbler = opt(nibbler, [&](Nibbler n) {
        tie(tmp, n) = parse_branch_else(n);
        branch_node.children.push_back(tmp);
        return n;
    });

    return {branch_node, nibbler};
}

// 'if' , expression , '{' , body , '}'
AST_Nib_Pair_t parse_branch_if(Nibbler nibbler) {
    AST_Node if_node(NODE_TYPE::BRANCH_IF);
    AST_Node tmp;

    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IF); // for the sake of getting a token
    if_node.tok = tmp.tok;
    tie(tmp, nibbler) = parse_expression(nibbler);
    if_node.children.push_back(tmp);

    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    if_node.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {if_node, nibbler};
}

// 'if else' , expression , '{' , body , '}'
AST_Nib_Pair_t parse_branch_if_else(Nibbler nibbler) {
    AST_Node if_else_node(NODE_TYPE::BRANCH_IF_ELSE);
    AST_Node tmp;

    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::IF_ELSE); // for the sake of getting a token
    if_else_node.tok = tmp.tok;
    tie(tmp, nibbler) = parse_expression(nibbler);
    if_else_node.children.push_back(tmp);

    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    if_else_node.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {if_else_node, nibbler};
}

// 'else' , '{' , body , '}'
AST_Nib_Pair_t parse_branch_else(Nibbler nibbler) {
    AST_Node else_node(NODE_TYPE::BRANCH_ELSE);
    AST_Node tmp;

    tie(tmp, nibbler) = require(nibbler, TOK_TYPE::ELSE); // for the sake of getting a token
    else_node.tok = tmp.tok;

    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    else_node.children.push_back(tmp);
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {else_node, nibbler};
}

//LOOPS

// while_loop | repeat_loop
AST_Nib_Pair_t parse_loop(Nibbler nibbler) {
    return alt({parse_while_loop, parse_repeat_loop}, nibbler);
}

// 'while' , expression , '{' , body , '}'
AST_Nib_Pair_t parse_while_loop(Nibbler nibbler) {
    AST_Node loop_node(NODE_TYPE::LOOP_WHILE);
    AST_Node tmp;

    nibbler = require(nibbler, TOK_TYPE::WHILE).second;

    tie(tmp, nibbler) = parse_expression(nibbler);
    loop_node.children.push_back(tmp);

    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    loop_node.children.push_back(tmp);   
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {loop_node, nibbler};
}

// 'repeat' , expression , '{' , body , '}'
AST_Nib_Pair_t parse_repeat_loop(Nibbler nibbler) {
    AST_Node loop_node(NODE_TYPE::LOOP_REPEAT);
    AST_Node tmp;

    nibbler = require(nibbler, TOK_TYPE::REPEAT).second;

    tie(tmp, nibbler) = parse_expression(nibbler);
    loop_node.children.push_back(tmp);

    nibbler = require(nibbler, TOK_TYPE::OPEN_CURLY).second;
    tie(tmp, nibbler) = parse_body(nibbler);
    loop_node.children.push_back(tmp);   
    nibbler = require(nibbler, TOK_TYPE::CLOSE_CURLY).second;

    return {loop_node, nibbler};
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
    nibbler = opt(nibbler, [&](Nibbler n) {
        tie(_, n) = require(n, TOK_TYPE::POW);
        if(_.type==NODE_TYPE::NON) //this will run no matter what it's just to please the compiler (it thinks this is an infinite recursion case but it really isn't I swear)
            tie(tmp, n) = parse_exp_pow(n);

        use_pow = true;
        return n;
    });

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
                [&](Nibbler n){ return alt_types({TOK_TYPE::STRING_LITERAL, TOK_TYPE::INT_LITERAL, TOK_TYPE::FLOAT_LITERAL, TOK_TYPE::TRUE, TOK_TYPE::FALSE}, nibbler); },
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