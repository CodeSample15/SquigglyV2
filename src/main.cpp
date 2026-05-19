#include <string>
#include "lex.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    string input = "hello #this is a comment\nhi";

    try {
        vector<Token> tokens = lex(input);
        lex_strip(tokens);
        print_tokens(tokens);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e, input);
    }

    return 0;
}