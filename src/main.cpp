#include <iostream>
#include <string>

#include "lex.hpp"
#include "parse.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    string input = "i++";

    try {
        cout << "Lexing..." << endl;
        vector<Token> tokens = lex(input);
        lex_strip(tokens);
        print_tokens(tokens);

        cout << endl;
        cout << endl;

        cout << "Parsing..." << endl;
        Nibbler nibbler(&tokens);
        AST_Node AST = parse_variable_assign(nibbler).first;
        print_AST(AST);
    }
    catch (ScribbleErr e) {
        PrintSErrMessage(e, input);
    }

    return 0;
}