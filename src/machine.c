#include "machine.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strarr.h"

void machine_init(
  machine_t* m, char** symbols, size_t symbol_count, char** states,
  size_t state_count, state_t default_state
) {
  m->symbol_count = symbol_count;
  m->symbols = strarr_copy(symbols, symbol_count);

  m->state_count = state_count;
  m->states = strarr_copy(states, state_count);

  m->rules = malloc(state_count * symbol_count * sizeof(rule_t));

  m->default_state = default_state;

  // By default: write to blank, do nothing and switch to halt state
  memset(m->rules, 0, state_count * symbol_count * sizeof(rule_t));
}

void machine_add_rule(
  machine_t* m, state_t state, symbol_t symbol, symbol_t write_symbol,
  move_t move_tape, state_t next_state
) {
  rule_t* rule = m->rules + (m->symbol_count * state + symbol);

  rule->write_symbol = write_symbol;
  rule->move_tape = move_tape;

  // Preserve 0 for explicit halt state
  rule->next_state = next_state + 1;
}

void machine_run(machine_t* m, const char* input) {
  tape_t tape;
  tape_init(&tape);

  const char* ptr = input;

  while (*ptr != '\0') {
    while (*ptr == ' ') ptr++;
    if (*ptr == '\0') break;
    const char* next = ptr + 1;

    while (*next != '\0' && *next != ' ') next++;
    size_t len = next - ptr;

    bool found = false;
    for (symbol_t i = 0; i < m->symbol_count; ++i) {
      if (len != strlen(m->symbols[i])) continue;
      if (memcmp(ptr, m->symbols[i], len)) continue;

      found = true;
      tape_write(&tape, i);
      tape_move_right(&tape);
      break;
    }

    if (!found) {
      fprintf(stderr, "ERROR: Unknown symbol: %.*s\n", (int)len, ptr);
      return;
    }
    ptr = next;
  }

  tape.head = tape.begin;
  state_t current_state = m->default_state + 1;

  for (;;) {
    size_t state_idx = m->symbol_count * (current_state - 1);
    rule_t* rule = m->rules + state_idx + tape_read(&tape);

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
  for (size_t i = tape.begin; i < tape.end; ++i) {
    printf("%s ", m->symbols[tape.data[i]]);
  }
  printf("\n -- %s --\n", m->states[current_state - 1]);

  tape_free(&tape);
}

void machine_free(machine_t* m) {
  free(m->rules);
  strarr_free(m->symbols);
  strarr_free(m->states);
}
