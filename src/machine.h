#ifndef MACHINE_H
#define MACHINE_H

#include <stdint.h>

#include "dumbset.h"
#include "tape.h"

typedef uint16_t state_t;

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

  char** symbols;
  char** states;

  rule_t* rules;
} machine_t;

extern machine_t machine_new(set_t, set_t);

extern void machine_add_rule(
  machine_t*, state_t, symbol_t, symbol_t, move_t, state_t
);

extern void machine_run(machine_t*, const char*);
extern void machine_free(machine_t*);

#endif
