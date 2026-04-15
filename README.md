# SquigglyV2
Doing this properly

## Language EBNF
```EBNF
program = {import_statement} , {core_function | function_def | variable_def | }

core_function       = start_func | update_func
start_func          = ':START:{' , {expression} , '}'
update_func         = ':UPDATE:{' , {expression} , '}'


# Function declarations:
VALID_FUNCTION_MODIFIER = 'thread' | 'quiet' |  # room to grow with more function modifiers
FUNCTION_KEYWORD        = 'fun'

function_def        = [function_modifier] , FUNCTION_KEYWORD , identifier , '(' , [parameters] , ')' , '{' , {expression} , '}'

function_modifier   = '[' , VALID_FUNCTION_MODIFIER , {',' , VALID_FUNCTION_MODIFIER} , ']'


# Function bodies:
expression          = (variable_def 
                    | variable_assign 
                    | branch 
                    | function_call
                    | loop) , [';']


# variables
variable_def        = VARTYPE , variable_reference , ['=' , expression]
variable_assign     = variable_reference , 
                        VARIABLE_IN_PLACE_MOD | (VARIABLE_MOD , expression)

VARIABLE_MOD        = '=' 
                    | '+=' 
                    | '-=' 
                    | '/=' 
                    | '*='
VARIABLE_IN_PLACE_MOD = '--' | '++'

VARTYPE             = ('num' | 'float' | CLASS_NAME) , [ARR_TYPE]
ARR_TYPE = '[' , expression, {',' , expression} , ']'
```

NB: Grammar assumes whitespace has been stripped from the output of the lexer 

## Demo

```rust
use my_library as lib
use folder/fancy_graphics as graph

num count = 0;
num lives = 5       # optional semicolons
OBJECT[5, 4] grid;

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
    while(true) {
        count++
    }
}
```