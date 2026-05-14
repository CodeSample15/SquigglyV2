#include <string>
#include "lex.hpp"
#include "err.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char** argv) {
    try {
        string input = "h3ll0";
        vector<Token> tokens = lex(input);
        print_tokens(tokens);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e);
    }

    return 0;
}