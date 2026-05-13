#include <string>
#include "lex.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    try {
        string input = "hello";
        vector<Token> tokens = lex(input);
        print_tokens(tokens);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e);
    }

    return 0;
}