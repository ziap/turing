#include "machine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void machine_init(
  machine_t* machine, const char** symbols, size_t symbol_count,
  const char** states, size_t state_count
) {
  machine->symbol_count = symbol_count;
  machine->symbols = symbols;

  machine->state_count = state_count;
  machine->states = states;

  machine->rules = malloc(state_count * symbol_count * sizeof(rule_t));

  // By default: write to blank, do nothing and switch to halt state
  memset(machine->rules, 0, state_count * symbol_count * sizeof(rule_t));
}

void machine_add_rule(
  machine_t* machine, state_t state, symbol_t symbol, symbol_t write_symbol,
  move_t move_tape, state_t next_state
) {
  rule_t* rule = machine->rules + (machine->symbol_count * state + symbol);

  rule->write_symbol = write_symbol;
  rule->move_tape = move_tape;

  // Preserve 0 for explicit halt state
  rule->next_state = next_state + 1;
}

static void machine_print_tape(machine_t* machine, tape_t* tape) {
  for (size_t i = tape->begin; i < tape->end; ++i) {
    printf("%s ", machine->symbols[tape->data[i]]);
  }

  printf("\n");
}

void machine_run(
  machine_t* machine, symbol_t* inputs, size_t input_size, state_t init_state
) {
  tape_t tape;
  tape_init(&tape);

  for (symbol_t* s = inputs; s != inputs + input_size; ++s) {
    tape_write(&tape, *s);
    tape_move_right(&tape);
  }

  tape.head = tape.begin;
  state_t current_state = init_state + 1;

  printf("INPUT:  ");
  machine_print_tape(machine, &tape);

  for (;;) {
    size_t state_idx = machine->symbol_count * (current_state - 1);
    rule_t* rule = machine->rules + state_idx + tape_read(&tape);

    if (rule->next_state == 0) break;

    tape_write(&tape, rule->write_symbol);

    switch (rule->move_tape) {
      case HOLD: break;
      case LEFT: tape_move_left(&tape); break;
      case RIGHT: tape_move_right(&tape); break;
    }

    current_state = rule->next_state;
  }

  printf("OUTPUT: ");
  machine_print_tape(machine, &tape);
  printf("-- %s --\n", machine->states[current_state - 1]);
}

void machine_free(machine_t* machine) {
  if (machine->rules) free(machine->rules);
}
