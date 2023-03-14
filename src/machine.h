#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#include "tape.h"

typedef uint32_t state_t;

typedef enum {
  HOLD,
  LEFT,
  RIGHT,
} move_t;

typedef struct {
  symbol_t write_symbol;
  state_t next_state;
  move_t move_tape;
} rule_t;

typedef struct {
  symbol_t symbol_count;
  state_t state_count;

  const char** symbols;
  const char** states;

  rule_t* rules;
} machine_t;

extern void machine_init(
  machine_t*, const char**, size_t, const char**, size_t
);

extern void machine_add_rule(
  machine_t*, state_t, symbol_t, symbol_t, move_t, state_t
);

extern void machine_run(machine_t*, symbol_t*, size_t, state_t);
extern void machine_free(machine_t*);

#endif
