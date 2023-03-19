#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stddef.h>

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
  const char* text;
  size_t length;
  size_t line;
  size_t col;
} token_t;

typedef struct {
  const char* content;
  const char* cursor;
  const char* end;
  size_t line;
  const char* line_start;
} lexer_t;

extern lexer_t lexer_new(const char*);
extern token_t lexer_next(lexer_t*);

#endif
