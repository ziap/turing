#ifndef PARSER_H
#define PARSER_H

#include "../common.h"
#include "dumbset.h"
#include "lexer.h"

typedef struct {
  uint16_t current;
  uint16_t read;
  uint16_t write;
  move_t dir;
  uint16_t next;
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

extern parser_t parser_new(const char*, sized_str_t);
extern bool parse_machine(parser_t*);
extern void parser_cleanup(parser_t*);

#endif
