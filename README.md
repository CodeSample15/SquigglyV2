# Scribble

Make really simple games through a language that's built for that very purpose. 

## Purpose and goals

Scribble's aim is to be similar to its precursor [Squiggly](https://github.com/CodeSample15/Squiggly) while being faster, more robust, and easier to develop. 

There are countless ways to develop video games and graphical applications. However, there's a few issues I have with modern methods that I aim to fix with Scribble:

 - Most languages require you to find libraries to create a graphical interface and input manager for their application. This can lead to version issues, annoying linker errors, and confusion for first time developers.
    - Scribble comes with built in features to handle this, making it so that the user can immediately focus on what goes on the screen, not how it does so.
 - Distributing applications
    - Scribble will aim to run code on as many platforms as possible... including web. This means that users can write code for one device and safely share their code with others.
 - Security issues
    - Scribble has no way to interface directly with local files, make unsafe system calls, or anything similar. This means that users can execute other's Scribble files immediately after downloading without fear of accidentally running malware.
- Fun and education
    - Modern day programming education is very focused around using LLMs to write code. In my opinion, this takes the joy out of learning to code, especially for new developers. This language aims to provide a simple syntax that is both fun to use and teaches new developers how to think like a programmer, without having to rely on prompting.
    - The idea isn't to develop the next hackathon-winning app with this language, but instead just focus on making something cool for others to enjoy, while learning to think programmatically along the way.

## Progress

### Lexer:

- [X] Lex all tokens needed for parser
- [X] Comprehensive error reporting
- [X] Tests
- [ ] Edge case handling (invalid float literals?)

### Parser:

- [ ] Program
- [ ] import_statement
- [ ] function_def
    - [ ] function_modifier
    - [ ] parameters
    - [ ] body
- [ ] variable_def
    - [ ] vartype
- [ ] Core function
    - [ ] start_func
    - [ ] update_func

### Interpreter:

*Not started*

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

## Example

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

==