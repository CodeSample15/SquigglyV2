#include <iostream>
#include <cstring>

#include "tests.hpp"
#include "lexer/lexer_tests.hpp"

using namespace std;

int main(int argc, char** argv) {
    if(argc != 0) {
        for(int i=0; i<argc; i++) {
            if(strcmp(argv[i], "lex")==0) {
                //run lexer tests
            }
        }
    } else {
        //run all tests
    }

    return 0;
}