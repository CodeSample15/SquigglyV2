#pragma once

#include <vector>
#include "lex.hpp"

enum NODE_TYPE {
    PROGRAM,
    IMPORT_STATEMENT, CORE_FUNCTION, FUNCTION_DEF, VARIABLE_DEF, VARIABLE_ASSIGN,
    START_FUNC, UPDATE_FUNC,

    VAR_TYPE,

    BODY, BRANCH, 

    FUNCTION_MODIFIER, FUNCTION_CALL, ARGUMENTS, PARAMETERS,

    IDENT,

    NON
};

struct AST_Node {
    NODE_TYPE type;
    Token tok;
    std::vector<AST_Node> children;

    AST_Node() {
        this->type=NON;
    }
    AST_Node(NODE_TYPE type) {
        this->type = type;
    }
};

