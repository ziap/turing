#ifndef MACHINE_H
#define MACHINE_H

#include "../common.h"

typedef struct {
  uint16_t write_symbol;
  uint16_t next_state;
  move_t move_dir;
} rule_t;

typedef struct {
  uint16_t symbol_count;
  uint16_t state_count;

  sized_str_t symbols;
  sized_str_t states;

  rule_t *rules;
} machine_t;

// Load a machine from a sequence of bytes, only references the bytes
extern bool load_machine(sized_str_t, machine_t *);

#endif
