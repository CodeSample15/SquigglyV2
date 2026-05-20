#include <string>
#include "lex.hpp"
#include "parse.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    string input = "num i = 4;";

    try {
        vector<Token> tokens = lex(input);
        lex_strip(tokens);
        print_tokens(tokens);

        Nibbler nibbler(&tokens);
        parse_variable_def(nibbler);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e, input);
    }

    return 0;
}