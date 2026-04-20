# SquigglyV2
Doing this properly

## Language Grammar
```EBNF
program = {import_statement} , {core_function | function_def | variable_def | }

core_function       = start_func | update_func
start_func          = ':START:{' , {body} , '}'
update_func         = ':UPDATE:{' , {body} , '}'


# Function declarations:
VALID_FUNCTION_MODIFIER = 'thread' | 'quiet' |  # room to grow with more function modifiers

function_def        = [function_modifier] , 'fun' , identifier , '(' , [parameters] , ')' , '{' , {body} , '}'

function_modifier   = '[' , VALID_FUNCTION_MODIFIER , {',' , VALID_FUNCTION_MODIFIER} , ']'

# function_call
function_call       =  identifier , '(' , [arguments] , ')'

parameters          = VARTYPE , identifier , {',' , VARTYPE , identifier}
arguments           = expression , {',' , expression}

# Function bodies:
body                = (variable_def 
                    | variable_assign 
                    | branch 
                    | function_call
                    | loop) , [';']

# variables
variable_def        = VARTYPE , identifier , {',' , identifier} , ['=' , expression]
variable_assign     = variable_reference , 
                        INCR_DECR_OP | (ASSIGN_OP , expression)

ASSIGN_OP           = '=' 
                    | '+=' 
                    | '-=' 
                    | '/=' 
                    | '*='
INCR_DECR_OP        = '--' | '++'

VARTYPE             = ('num' | 'float' | 'string' | CLASS_NAME) , [ARR_TYPE]
ARR_TYPE            = '[' , expression, {',' , expression} , ']'

# branches
branch = branch_if ,  {branch_ifelse} , [branch_else]

branch_if           = 'if' , expression , '{' , [body] , '}'
branch_ifelse       = 'if else' , expression , '{' , [body] , '}'
branch_else         = 'else' , '{' , [body] , '}'

# expression
expression          = 
```

NB: Grammar assumes whitespace has been stripped from the output of the lexer 

## Demo

```rust
use my_library as lib
use folder/fancy_graphics as graph

num count = 0;
num lives = 5       # optional semicolons
OBJECT[5, 4] grid;  # arrays are fixed-size C# style dynamically allocated arrays

:START: {
    lives += 1
}

:UPDATE: {
    if count > 4 {
        count = 0
        lives--
    }
}

[thread]
fun coroutine() {
    # can run parallel to main program
    while(true) {
        count++
    }
}
```