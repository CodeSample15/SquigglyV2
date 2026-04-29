#include <string>
#include "lex.hpp"
#include "err.hpp"

using namespace std;

int main(int argc, char** argv) {
    try {
        string input = "t";
        lex(input);
    } 
    catch (ScribbleErr e) {
        PrintSErrMessage(e);
    }

    return 0;
}