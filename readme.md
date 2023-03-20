# Turing 

Simple turing machine emulator in C

## Features

- [x] Interpreter
- [x] Bytecode format
- [x] Description language
- [ ] Pre-processor (sub states, pattern matching, ...)
- [ ] Visualizer (visualize the execution, display graph representation, ...)

## Usage

Compile using the `build.sh` script.

Run `./turing help` for more information

Example:

```yaml
$ ./turing compile examples/add2.tr add2.tb
$ ./turing run add2.tb  
INPUT:  1 0 0 0 1 0 + 1 0 0 0 1 1
OUTPUT: 1 0 0 0 1 0 1 
 -- HALT --
```

## Creating turing machines

Documentation for the description language is coming soon. For the time being,
you can take a look at the examples to get a brief understanding of how to
create turing machines.

## License

This project is licensed under the [MIT license](LICENSE).
