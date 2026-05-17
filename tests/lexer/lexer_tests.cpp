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
    //testing literals
    tests.emplace_back("Identifier 1", [&]{ test_type_lex("x", TOK_TYPE::IDENTIFIER, "x"); });
    tests.emplace_back("Identifier 2", [&]{ test_type_lex("h3ll0", TOK_TYPE::IDENTIFIER, "h3ll0"); });

    tests.emplace_back(TEST_NAME_FOR_SPACE, []{});

    tests.emplace_back("Digit 1", [&]{ test_type_lex("23", TOK_TYPE::INT_LITERAL, "23"); });
    tests.emplace_back("Digit 2", [&]{ test_type_lex("23.3", TOK_TYPE::FLOAT_LITERAL, "23.3"); });
    tests.emplace_back("Digit 3", [&]{ test_type_lex("23.", TOK_TYPE::FLOAT_LITERAL, "23."); });
    tests.emplace_back("Digit 4", [&]{ test_type_lex("23.3", TOK_TYPE::FLOAT_LITERAL, "23.3"); });

    tests.emplace_back(TEST_NAME_FOR_SPACE, []{});

    tests.emplace_back("String literal", [&]{ test_type_lex("\"Hello World!\"", TOK_TYPE::STRING_LITERAL, "\"Hello World!\""); });
    tests.emplace_back("String literal - unclosed", [&]{ test_type_lex("\"Hello World!", TOK_TYPE::STRING_LITERAL, "", true); });
    tests.emplace_back("String literal - newline unclosed", [&]{ test_type_lex("\"Hello Wor\nld!\"", TOK_TYPE::STRING_LITERAL, "", true); });

    tests.emplace_back(TEST_NAME_FOR_SPACE, []{});

    //testing keywords
    tests.emplace_back("keyword: use", [&]{ test_type_lex("use", TOK_TYPE::USE, "use"); });
    tests.emplace_back("keyword: as", [&]{ test_type_lex("as", TOK_TYPE::AS, "as"); });
    tests.emplace_back("keyword: if", [&]{ test_type_lex("if", TOK_TYPE::IF, "if"); });
    tests.emplace_back("keyword: if else", [&]{ test_type_lex("if else", TOK_TYPE::IF_ELSE, "if else"); });
    tests.emplace_back("keyword: else", [&]{ test_type_lex("else", TOK_TYPE::ELSE, "else"); });
    tests.emplace_back("keyword: fun", [&]{ test_type_lex("fun", TOK_TYPE::FUNCTION_DEFINE, "fun"); });
    tests.emplace_back("keyword: num", [&]{ test_type_lex("num", TOK_TYPE::NUMBER_TYPE, "num"); });
    tests.emplace_back("keyword: float", [&]{ test_type_lex("float", TOK_TYPE::FLOAT_TYPE, "float"); });
    tests.emplace_back("keyword: string", [&]{ test_type_lex("string", TOK_TYPE::STRING_TYPE, "string"); });

    tests.emplace_back(TEST_NAME_FOR_SPACE, []{});
}