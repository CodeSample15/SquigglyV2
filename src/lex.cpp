#include "lex.hpp"
#include "file_reader.hpp"
#include "err.hpp"

using namespace std;

//helper prototypes
TOK_TYPE handle_alpha(file_reader::file_reader &fr, string &lexeme);
TOK_TYPE handle_digit(file_reader::file_reader &fr, string &lexeme);
TOK_TYPE check_is_keyword(string &lexeme, file_reader::file_reader &fr);

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

        char c = fr.peek();
        if(isalpha(c))
            type = handle_alpha(fr, lexeme);
        else if(isdigit(c))
            type = handle_digit(fr, lexeme);
        else
            fr.next();

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

    while(!fr.empty() && (isalpha(fr.peek()) || isdigit(fr.peek()))) {
        lexeme += fr.next();
    }

    return check_is_keyword(lexeme, fr);
}

TOK_TYPE handle_digit(file_reader::file_reader &fr, string &lexeme) {
    lexeme += fr.next();

    bool found_dot = false;

    while(!fr.empty() && (isdigit(fr.peek()) || fr.peek() == '.')) {
        char next = fr.peek();
        if(next == '.') {
            if(found_dot)
                return TOK_TYPE::FLOAT_LITERAL; //if we already found a dot, return what we currently found
            else
                found_dot = true;
        }

        lexeme += fr.next();
    }

    return found_dot ? TOK_TYPE::FLOAT_LITERAL : TOK_TYPE::INT_LITERAL;
}

/*
    Check if a string of characters is a keyword in the language or just a regular identifier
*/
TOK_TYPE check_is_keyword(string &lexeme, file_reader::file_reader &fr) {
    if(lexeme == "use")
        return TOK_TYPE::USE;
    else if(lexeme == "as")
        return TOK_TYPE::AS;
    else if(lexeme == "if") {
        //check for else in "if else"
        if(fr.has_next(" else")) return TOK_TYPE::IF_ELSE;
        return TOK_TYPE::IF;
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