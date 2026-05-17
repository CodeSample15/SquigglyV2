#include <windows.h>
#include <iostream>
#include <cstring>

#include "tests.hpp"
#include "Colors.hpp"
#include "lexer/lexer_tests.hpp"

using namespace std;

int main(int argc, char** argv) {
    SetConsoleOutputCP(CP_UTF8);

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

    int failed_tests = 0;
    int total_tests = 0;

    //run tests
    for(test_t &test : tests) {
        if(test.name == TEST_NAME_FOR_SPACE) {
            cout << "--------------------------------" << endl;
            continue;
        }

        total_tests++;

        try {
            test.test_fun();

            cout << "[ ✅ ] Test passed! -- \"" << test.name << "\"" << endl;
        } catch (test_fail &err) {
            failed_tests++;

            cout << "[ ❌ ] Test failed: -- \"" << test.name << "\"" << endl;
            cout << "\t\t↳ [" << KCYN << err.test_name << RST << "] " << "\"" << err.reason << "\"" << endl;
        }
    }

    cout << endl;
    cout << "[-] " << FMAG("Tests completed:") << endl;
    cout << FGRN("\t↳ Passed: ") << (total_tests - failed_tests) << endl;
    cout << FRED("\t↳ Failed: ") << failed_tests << endl;

    return 0;
}