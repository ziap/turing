#ifndef LEXER_H
#define LEXER_H

#include "../common.h"

typedef enum {
  TOKEN_COMMENT,
  TOKEN_LCURLY,
  TOKEN_RCURLY,
  TOKEN_ARROW,
  TOKEN_IDEN,
  TOKEN_NEWLINE,
  TOKEN_EOF,
  TOKEN_UNKNOWN,
  TOKEN_COUNT,
} token_type_t;

extern const char* token_display[TOKEN_COUNT];

typedef struct {
  token_type_t type;
  sized_str_t text;
  size_t line;
  size_t col;
} token_t;

typedef struct {
  char* content;
  char* cursor;
  const char* end;
  size_t line;
  const char* line_start;
} lexer_t;

extern lexer_t lexer_new(sized_str_t);
extern token_t lexer_next(lexer_t*);

#endif
