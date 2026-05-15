#include <iostream>
#include <cstring>

#include "tests.hpp"
#include "lexer/lexer_tests.hpp"

using namespace std;

int main(int argc, char** argv) {
    vector<test_t> tests;

    if(argc != 1) {
        for(int i=0; i<argc; i++) {
            if(strcmp(argv[i], "lex")==0)
                load_lexer_tests(tests);
        }
    } else {
        //load all tests
        load_lexer_tests(tests);
    }

    //run tests
    for(test_t &test : tests) {
        try {
            test.test_fun();

            cout << "[] Test passed!" << endl;
        } catch (test_fail &err) {
            cout << "[] Test failed:" << endl;
            cout << "       + \"" << err.test_name << "\" --> " << err.reason << endl;
        }
    }

    return 0;
}