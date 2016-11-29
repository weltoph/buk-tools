# while-parser
This is a project to implement a interpreter for the while-programming language
(this language is very simple but Turing complete and thus, interesting for
theoretical examination). In the Python folder you can use parser.py to run it
against the json files. (The format *should* be self explanatory).

## The Rewrite
I rewrote this project to obtain a more robust interpreter. Therefore the
parser is implemented by using flex and bison.
Building is - for now - done manually: run bison with the -d flag against
while.y and afterwards flex against while.l and compile with gcc -lfl
prog.c lex.yy.c while.tab.c. The binary takes as first argument the code file
with your while program and afterwards the input parameter.

