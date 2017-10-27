# Description
This repository is meant to collect some useful tools for the lecture
["Berechenbarkeit und Komplexität"](https://algo.rwth-aachen.de/Lehre/WS1718/BuK.php).
It is planned to include an interpreter for the following Turing-complete
models: 
* while
* Turingmachines
* ...

# Disclaimer
Note that this is only for educational purposes and not safe to use for anything but playing around with (and even for this I can not give any guarantees...!). Thus, proceed at your own risk.

# While Interpreter (whint)
The while interpreter can be build with `make while` and produces an executable
`whint` in the `while` subdirectory. This executable takes as first argument
the path to a while program (see `while/progs` for examples) and all following
paramters are parsed as inputs to the program and stored in the starting
registers.

# Turingmachine Interpreter (tmint)
Can be build with `make tm` which produces an executable `tmint`.

