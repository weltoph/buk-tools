# Description
This repository is meant to collect some useful tools for the lecture
["Berechenbarkeit und Komplexität"](https://algo.rwth-aachen.de/Lehre/WS1718/BuK.php).
Specifically, it provides simulators for some of the basic computation models
which are examined within the lecture. By now the following models are
implemented:
* while-language
* Turingmachines
These implementations are described in more detail below.

# Disclaimer
All the software contained in this repository is meant for educational purposes
only. Also it is implemented with good intentions but questionable skill. Therefore
I want to issue a warning to proceed at your own risk and with caution!

# Contributions
Contributions are welcome and encouraged. It is planned to implement further
programming models and maybe a few esoteric programming languages, e.g.
* Register-machines
* loop-language
* Brainfuck
* ...
Also a few more examples for the implemented programming languages can be
contributed, e.g. for solving some common programming tasks:
* basic mathematical operations (addition, multiplication, square root, ...) in
binary or decimal system
* implementing characteristic functions for some languages (binary string that
are palindromes, ...)
Other maintenance tasks are always open which include but are not limited to:
* improving the user interfaces of the programs
* adding documentation (for code but also descriptions for the models)
* ...

# While Interpreter (whint)
The while-language builds around a simple control flow construct which is
`while xN /= 0 do S end;` where `N` denotes a natural number and `S` the further
program. The second construct that can be used is an assignment of the form
`xN := xN + c` where `c` denotes one element of `{+1, 0, -1}`. All variables
hold natural numbers (theoretically, practically the values from 0-255). The
input/output behaviour is given by the following conventions: the input values
are provided in the first `x1, x2, ..., xN` variables and after the computation
the variable `x0` is given as output.

## Grammar
```
prog ::= prog; prog | xN := xN + c | while xN /= 0 do prog end
N    ::= 0 | [1-9]+ /* only up to index 100 */
c    ::= -1 | 0 | +1
```
with the obvious semantic.

## User Interface
In the root directory `make while` builds the executable in `while/whint`.
The interface simply takes as first argument the path to a text file which
defines a while program and every further argument is stored in the
corresponding variables `x1, ..., xN`. For example `whint program.while 0 1`.

# Turingmachine Interpreter (tmint)
A Turingmachine is a tupel `(Q, \Sigma, \Gamma, B, q0, q, \delta)` with a set
of states `Q`, a set of input/output symbols `\Sigma`, a set of tape symbols
`\Gamma`, a blank symbol `B` which is `\Gamma' but not in `\Sigma`, a start
state `q0` and end state `q` from `Q` and a transition function
`\delta: Qx\Gamma -> Qx\Gamma x {L, R, N}`. The input/output behaviour is
defined as follows: in the beginning the head position of the Turingmachine
points to the first element of the input which is written in the following
cells of the tape. All other tapecells contain `B`. At the moment the
Turingmachine moves to the end state the output is given from the tapecell the
head points to to the first tapecell to the right that does not contain an
element of input/output alphabet. Further explanations can be found in the
[wikipedia article](https://en.wikipedia.org/wiki/Turing_machine) and a more
physical representation see this
[video](https://www.youtube.com/watch?v=E3keLeMwfHY)

## Grammar
```
spec ::= { identifiers }; { identifiers }; { identifiers }; ident; ident; ident; {transitions}
ident ::= [a-z0-9]+
identifiers ::= [identifiers,] ident
transitions ::= [transitions,] transition
transition ::= (ident, ident) -> (ident, ident, X)
X ::= L | R | N
```
where the semantic of `spec` follows the tupel from above and a transition of
the form `(q, a) -> (p, b, X)` defines `\delta(q, a) = (p, b, X)`.
Not defined behaviour or `\delta` defaults to a neutral operation (i.e.
`(q, a) -> (q, a, N)`) which implies an endless loop in the Turingmachine.

## User Interface
In the root directory `make tm` builds the executable in `tm/tmint`.
The interface simply takes as first argument the path to a text file which
defines a Turingmachine and every further argument are written to the tape.
For example `tmint machine.tm 0 1 1 0` generates the following configuration:
`B q0 0 1 1 0 B`.
