#include "lex.hpp" //from main project
#include "lexer_tests.hpp"

using namespace std;

void test_type_lex(string input, TOK_TYPE expected_type, string expected_lexeme) {
    vector<Token> out = lex(input);

    if(out.size() == 0) throw (test_fail) {"Type/Lexeme test", ""};
}


void load_lexer_tests(vector<test_t> &tests) {
    //tests.emplace_back("");
}