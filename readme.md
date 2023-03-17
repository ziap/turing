# Turing 

Simple turing machine emulator in C

## Features

- [x] Interpreter
- [x] Bytecode format
- [ ] Description language
- [ ] Pre-processor (sub states, pattern matching, ...)
- [ ] Visualizer

## Usage

Compile using the `build.sh` script.

Run the examples

```yaml
$ ./turing run examples/add2.tb  
INPUT:  1 0 0 0 1 0 + 1 0 0 0 1 1
OUTPUT: 1 0 0 0 1 0 1 
 -- HALT --
```

## Creating turing machines

Current, the only way to define a turing machine is to write C some code and
compile it. A compiler for the description alnguage is in development.

For example, a turing machine for adding two binary number:

```c
#include "src/machine.h"
#include "src/serializer.h"

int main(void) {
  enum { GO_TO_END, CHECK_LAST_NUMBER, GO_PLUS_MARK, GO_PLUS_ADD, GO_TO_MARK, GO_TO_ADD, MARK, CARRY, CLEANUP, HALT, STATE_COUNT };
  enum { BLANK, ZERO, ONE, PLUS, ADDED_ZERO, ADDED_ONE, SYMBOL_COUNT };

  char* states[STATE_COUNT] = {"GO TO END", "CHECK LAST NUMBER", "GO PLUS MARK", "GO PLUS ADD", "GO TO MARK", "GO TO ADD", "MARK", "CARRY", "CLEANUP", "HALT"};
  char* symbols[SYMBOL_COUNT] = {"", "0", "1", "+", "@0", "@1"};

  machine_t m;
  machine_init(&m, symbols, SYMBOL_COUNT, states, STATE_COUNT, GO_TO_END);

  machine_add_rule(&m, GO_TO_END, ZERO, ZERO, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_END, ONE, ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_END, ADDED_ZERO, ADDED_ZERO, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_END, ADDED_ONE, ADDED_ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_END, PLUS, PLUS, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_END, BLANK, BLANK, LEFT, CHECK_LAST_NUMBER);

  machine_add_rule(&m, CHECK_LAST_NUMBER, ZERO, BLANK, LEFT, GO_PLUS_MARK);
  machine_add_rule(&m, CHECK_LAST_NUMBER, ONE, BLANK, LEFT, GO_PLUS_ADD);
  machine_add_rule(&m, CHECK_LAST_NUMBER, PLUS, BLANK, LEFT, CLEANUP);

  machine_add_rule(&m, GO_PLUS_MARK, ZERO, ZERO, LEFT, GO_PLUS_MARK);
  machine_add_rule(&m, GO_PLUS_MARK, ONE, ONE, LEFT, GO_PLUS_MARK);
  machine_add_rule(&m, GO_PLUS_MARK, PLUS, PLUS, LEFT, GO_TO_MARK);

  machine_add_rule(&m, GO_PLUS_ADD, ZERO, ZERO, LEFT, GO_PLUS_ADD);
  machine_add_rule(&m, GO_PLUS_ADD, ONE, ONE, LEFT, GO_PLUS_ADD);
  machine_add_rule(&m, GO_PLUS_ADD, PLUS, PLUS, LEFT, GO_TO_ADD);

  machine_add_rule(&m, GO_TO_MARK, ZERO, ADDED_ZERO, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_MARK, BLANK, ADDED_ZERO, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_MARK, ONE, ADDED_ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_MARK, ADDED_ZERO, ADDED_ZERO, LEFT, GO_TO_MARK);
  machine_add_rule(&m, GO_TO_MARK, ADDED_ONE, ADDED_ONE, LEFT, GO_TO_MARK);

  machine_add_rule(&m, GO_TO_ADD, ZERO, ADDED_ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_ADD, BLANK, ADDED_ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, GO_TO_ADD, ONE, ADDED_ZERO, LEFT, CARRY);
  machine_add_rule(&m, GO_TO_ADD, ADDED_ZERO, ADDED_ZERO, LEFT, GO_TO_ADD);
  machine_add_rule(&m, GO_TO_ADD, ADDED_ONE, ADDED_ONE, LEFT, GO_TO_ADD);

  machine_add_rule(&m, CARRY, ZERO, ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, CARRY, BLANK, ONE, RIGHT, GO_TO_END);
  machine_add_rule(&m, CARRY, ONE, ZERO, LEFT, CARRY);

  machine_add_rule(&m, CLEANUP, ADDED_ZERO, ZERO, LEFT, CLEANUP);
  machine_add_rule(&m, CLEANUP, ADDED_ONE, ONE, LEFT, CLEANUP);
  machine_add_rule(&m, CLEANUP, ZERO, ZERO, HOLD, HALT);
  machine_add_rule(&m, CLEANUP, ONE, ONE, HOLD, HALT);
  machine_add_rule(&m, CLEANUP, BLANK, BLANK, HOLD, HALT);

  save_machine(&m, "examples/add2.tb");
  machine_free(&m);
}
```

## Building

On Linux

```
./build.sh
```

## License

This project is licensed under the [MIT license](LICENSE).
