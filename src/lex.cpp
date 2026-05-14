#include "lex.hpp"
#include "err.hpp"

using namespace std;

//simple struct to pass around to different functions to keep track of where we are in the source code
struct src_loc {
    size_t row;
    size_t col;
};

//helper prototypes
TOK_TYPE handle_alpha(string::iterator &it, src_loc &loc, string &lexeme);
TOK_TYPE check_is_keyword(string &lexeme);

vector<Token> lex(string &source) 
{
    //where we're going to store everything
    vector<Token> tokens;

    string::iterator it = source.begin();
    src_loc loc = {0,0}; //keep track of where we are in this file
    string lexeme = "";

    while(it < source.end()) 
    {
        //reset lexeme
        lexeme = "";

        //get what type of token we should have
        TOK_TYPE type = TOK_TYPE::OTHER;
        size_t start_col = loc.col;

        if(isalpha(*it))
            type = handle_alpha(it, loc, lexeme);
        else if(isdigit(*it)) {

        }

        //construct and push token
        Token next;
        next.line = loc.row;
        next.start_col = start_col;
        next.end_col = loc.col;
        next.lexeme = lexeme;
        next.type = type;

        tokens.push_back(next);
    }

    return tokens;
}

TOK_TYPE handle_alpha(string::iterator &it, src_loc &loc, string &lexeme) {
    lexeme += *(it++);
    loc.col++;

    while(isalpha(*it) || isdigit(*it)) {
        lexeme += *(it++);
        loc.col++;
    }

    return check_is_keyword(lexeme);
}

/*
    Check if a string of characters is a keyword in the language or just a regular identifier
*/
TOK_TYPE check_is_keyword(string &lexeme) {
    if(lexeme == "use")
        return TOK_TYPE::USE;
    else if(lexeme == "as")
        return TOK_TYPE::AS;
    else if(lexeme == "if") {
        //check for else in "if else"
    }
    else if(lexeme == "else")
        return TOK_TYPE::ELSE;
    else if(lexeme == "fun")
        return TOK_TYPE::FUNCTION_DEFINE;
    else if(lexeme == "num")
        return TOK_TYPE::NUMBER_TYPE;
    else if(lexeme == "float")
        return TOK_TYPE::FLOAT_TYPE;
    else if(lexeme == "string")
        return TOK_TYPE::STRING_TYPE;
    
    return TOK_TYPE::IDENTIFIER;
}