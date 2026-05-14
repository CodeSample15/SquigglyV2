#include "lex.hpp"
#include "file_reader.hpp"
#include "err.hpp"

using namespace std;

//helper prototypes
TOK_TYPE handle_alpha(file_reader::file_reader &fr, string &lexeme);
TOK_TYPE check_is_keyword(string &lexeme);

vector<Token> lex(string &source) 
{
    //where we're going to store everything
    vector<Token> tokens;

    file_reader::file_reader fr(&source);
    string lexeme = "";

    while(!fr.empty()) 
    {
        //reset lexeme
        lexeme = "";

        //get what type of token we should have
        TOK_TYPE type = TOK_TYPE::OTHER;
        size_t start_col = fr.loc.col;

        try {
            char next = fr.peek();
            if(isalpha(next))
                type = handle_alpha(fr, lexeme);
            else if(isdigit(next)) {

            }
        } catch(file_reader::read_err err) {
            continue;
        }

        //construct and push token
        Token next;
        next.line = fr.loc.row;
        next.start_col = start_col;
        next.end_col = fr.loc.col;
        next.lexeme = lexeme;
        next.type = type;

        tokens.push_back(next);
    }

    return tokens;
}

TOK_TYPE handle_alpha(file_reader::file_reader &fr, string &lexeme) {
    lexeme += fr.next();

    try {
        while(isalpha(fr.peek()) || isdigit(fr.peek())) {
            lexeme += fr.next();
        }
    } catch (file_reader::read_err) {} //safe to ignore

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