#include <iostream>
#include "debug.hpp"

using namespace std;

string tok_type_to_string(TOK_TYPE tok);

void print_tokens(vector<Token> &tokens) 
{
    for(Token& tok : tokens) {
        cout << "| " << tok_type_to_string(tok.type) << " | -- " << "line: " << tok.line << " -- cols: (" << tok.start_col << ", " << tok.end_col << " -- [" << tok.lexeme << "]" << endl;
    }
}

string tok_type_to_string(TOK_TYPE tok) {
    switch (tok) {
        case USE: return "use";
        case AS: return "as";
        case IF: return "if";
        case IF_ELSE: return "if else";
        case ELSE: return "else";
        case FUNCTION_DEFINE: return "function define";
        case NUMBER_TYPE: return "num type";
        case FLOAT_TYPE: return "float type";
        case STRING_TYPE: return "string type";
        case IDENTIFIER: return "identifier";
        case STRING_LITERAL: return "string literal";
        case INT_LITERAL: return "int literal";
        case FLOAT_LITERAL: return "float literal";
        case OPEN_PAREN: return "open paren";
        case CLOSE_PAREN: return "close paren";
        case OPEN_CURLY: return "open curly";
        case CLOSE_CURLY: return "close curly";
        case OPEN_BRACKET: return "open bracket";
        case CLOSE_BRACKET: return "close bracket";
        case SEMICOLON: return "semicolon";
        case COMMA: return "comma";
        case PLUS: return "plus";
        case MINUS: return "minus";
        case SLASH: return "slash";
        case STAR: return "star";
        case BAR: return "bar";
        case UP_ARROW: return "up arrow";
        case GREATER_THAN: return "greater than";
        case LESS_THAN: return "less than";
        case SHIFT_RIGHT: return "shift right";
        case SHIFT_LEFT: return "shift left";
        case EQUALS: return "equals";
        case PLUS_EQUALS: return "plus equals";
        case MINUS_EQUALS: return "minus equals";
        case SLASH_EQUALS: return "slash equals";
        case STAR_EQUALS: return "star equals";
        case INCR: return "incr";
        case DECR: return "decr";
        case IMAGE_REF: return "image ref";
        case BUILT_IN_VARIABLE_REF: return "built in variable ref";
        case SPECIAL_FUNCTION_PREFIX: return "special function prefix";
        case AND: return "and";
        case OR: return "or";
        case NOT: return "not";
        case WHITESPACE: return "whitespace";
        case COMMENT: return "comment";
        case OTHER: return "other";
        default: return "TOK NOT FOUND";
    }
}