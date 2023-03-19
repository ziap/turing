#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>

#include "dumbset.h"
#include "lexer.h"
#include "machine.h"

typedef struct {
  state_t current;
  symbol_t read;
  symbol_t write;
  move_t dir;
  state_t next;
} line_t;

typedef struct {
  size_t length;
  size_t capacity;
  line_t* data;
} lines_t;

typedef struct {
  const char* file_name;

  set_t states;
  set_t symbols;

  lines_t lines;

  lexer_t lexer;
} parser_t;

extern bool parse_machine(const char*, const char*, machine_t*);

#endif
