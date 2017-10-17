# Description
This repository is meant to collect some useful tools for the lecture
["Berechenbarkeit und Komplexität"](https://algo.rwth-aachen.de/Lehre/WS1718/BuK.php).
It is planned to include an interpreter for the following Turing-complete
models: 
* while
* Turingmachines
* ...

## While Interpreter (whint)
The while interpreter can be build with `make while` and produces an executable
`whint` in the `while` subdirectory. This executable takes as first argument
the path to a while program (see `while/progs` for examples) and all following
paramters are parsed as inputs to the program and stored in the starting
registers.

