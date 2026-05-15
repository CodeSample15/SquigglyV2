#include "../../include/lex.hpp"
#include "../../include/debug.hpp"
#include "lexer_tests.hpp"

using namespace std;

void test_type_lex(string input, TOK_TYPE expected_type, string expected_lexeme) {
    vector<Token> out = lex(input);

    if(out.size() == 0) 
        throw (test_fail) {"Type/Lexeme test", "Empty output"};
    if(out[0].type != expected_type)
        throw (test_fail) {"Type/Lexeme test", "Expected type |" + tok_type_to_string(expected_type) + "|, got |" + tok_type_to_string(out[0].type) + "|"};
    if(out[0].lexeme != expected_lexeme)
        throw (test_fail) {"Type/Lexeme test", "Expected lexeme '" + expected_lexeme + "', got '" + out[0].lexeme + "'"};
}


void load_lexer_tests(vector<test_t> &tests) {
    tests.emplace_back("Identifier 1", [&]{ test_type_lex("x", TOK_TYPE::IDENTIFIER, "y"); });
}