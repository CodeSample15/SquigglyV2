#include "../../include/lex.hpp"
#include "../../include/debug.hpp"
#include "../../include/err.hpp"
#include "lexer_tests.hpp"

using namespace std;

void test_type_lex(string input, TOK_TYPE expected_type, string expected_lexeme, bool should_fail=false) {
    vector<Token> out;

    try {
        out = lex(input);

        if(should_fail) throw (test_fail) {"Type/Lexeme test", "Lex didn't fail when it should have.\n"};
    } catch(ScribbleErr &e) {
        if(should_fail) return;
        PrintSErrMessage(e);
        throw (test_fail) {"Type/Lexeme test", "Lex failed"};
    }

    if(out.size() == 0) 
        throw (test_fail) {"Type/Lexeme test", "Empty output"};
    if(out[0].type != expected_type)
        throw (test_fail) {"Type/Lexeme test", "Expected type |" + tok_type_to_string(expected_type) + "|, got |" + tok_type_to_string(out[0].type) + "|"};
    if(out[0].lexeme != expected_lexeme)
        throw (test_fail) {"Type/Lexeme test", "Expected lexeme '" + expected_lexeme + "', got '" + out[0].lexeme + "'"};
}


void load_lexer_tests(vector<test_t> &tests) {
    tests.emplace_back("Identifier 1", [&]{ test_type_lex("x", TOK_TYPE::IDENTIFIER, "x"); });
    tests.emplace_back("Identifier 2", [&]{ test_type_lex("h3ll0", TOK_TYPE::IDENTIFIER, "h3ll0"); });

    tests.emplace_back("Digit 1", [&]{ test_type_lex("23", TOK_TYPE::INT_LITERAL, "23"); });
    tests.emplace_back("Digit 2", [&]{ test_type_lex("23.3", TOK_TYPE::FLOAT_LITERAL, "23.3"); });
    tests.emplace_back("Digit 3", [&]{ test_type_lex("23.", TOK_TYPE::FLOAT_LITERAL, "23."); });
    tests.emplace_back("Digit 4", [&]{ test_type_lex("23.3", TOK_TYPE::FLOAT_LITERAL, "23.3"); });
}