#include <string>
#include "lex.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    string input = "(){}[]&";

    try {
        vector<Token> tokens = lex(input);
        print_tokens(tokens);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e, input);
    }

    return 0;
}