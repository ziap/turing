#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

#define LINES_INIT_CAP 16
static lines_t lines_new(void) {
  lines_t lines = {
    .length = 0,
    .capacity = LINES_INIT_CAP,
    .data = malloc(LINES_INIT_CAP * sizeof(lines_t)),
  };

  return lines;
}

static void lines_add(lines_t* lines, line_t line) {
  if (lines->length == lines->capacity) {
    lines->capacity *= 2;
    lines->data = realloc(lines->data, lines->capacity * sizeof(lines_t));
  }
  lines->data[lines->length++] = line;
}

static void lines_free(lines_t* lines) { free(lines->data); }

// TODO: More specific error messages
static void error_wrong_token(
  const char* file_name, token_t token, token_type_t expecting
) {
  fprintf(
    stderr, "%s:%lu:%lu: ERROR: Expecting `%s`, found `%s`\n", file_name,
    token.line, token.col, token_display[expecting], token_display[token.type]
  );
}

static bool expect_token(parser_t* parser, token_t* token, token_type_t type) {
  token_t tmp = lexer_next(&parser->lexer);
  if (tmp.type != type) {
    error_wrong_token(parser->file_name, tmp, type);
    return false;
  }

  *token = tmp;
  return true;
}

static bool parse_line(parser_t* parser, token_t token, state_t s) {
  if (token.type == TOKEN_COMMENT) {
    if (!expect_token(parser, &token, TOKEN_NEWLINE)) return false;

    // A commented line is still a succesfully parsed line
    return true;
  }

  // The same goes to an empty line
  if (token.type == TOKEN_NEWLINE) { return true; }

  if (token.type != TOKEN_IDEN) {
    error_wrong_token(parser->file_name, token, TOKEN_IDEN);
    return false;
  }

  symbol_t r = set_find_and_add(&parser->symbols, token.text, token.length);

  if (!expect_token(parser, &token, TOKEN_IDEN)) return false;
  symbol_t w = set_find_and_add(&parser->symbols, token.text, token.length);

  if (!expect_token(parser, &token, TOKEN_ARROW)) return false;
  move_t dir;

  if (token.length > 1) goto invalid_arrow;

  switch (token.text[0]) {
    case '<': dir = LEFT; break;
    case '>': dir = RIGHT; break;
    case '-': dir = HOLD; break;
    default: goto invalid_arrow;
  }

  if (!expect_token(parser, &token, TOKEN_IDEN)) return false;
  state_t n = set_find_and_add(&parser->states, token.text, token.length);

  // Handle comment after end of line
  token = lexer_next(&parser->lexer);
  if (token.type == TOKEN_COMMENT) token = lexer_next(&parser->lexer);
  if (token.type != TOKEN_NEWLINE) {
    error_wrong_token(parser->file_name, token, TOKEN_NEWLINE);
    return false;
  }

  line_t line = {.current = s, .read = r, .write = w, .dir = dir, .next = n};
  lines_add(&parser->lines, line);

  return true;

invalid_arrow:
  fprintf(
    stderr, "%s:%lu:%lu: ERROR: Invalid arrow `%.*s`\n", parser->file_name,
    token.line, token.col, (int)token.length, token.text
  );
  return false;
}

static bool parse_block(parser_t* parser, state_t s) {
  // Consume the '{'
  token_t token = lexer_next(&parser->lexer);

  // Force line break after '{'
  while (token.type != TOKEN_NEWLINE) {
    if (token.type == TOKEN_COMMENT) {
      token = lexer_next(&parser->lexer);
      continue;
    }

    error_wrong_token(parser->file_name, token, TOKEN_NEWLINE);
    return false;
  }

  token = lexer_next(&parser->lexer);
  while (token.type != TOKEN_RCURLY) {
    if (!parse_line(parser, token, s)) return false;
    token = lexer_next(&parser->lexer);
  }

  // Consume the '}'
  token = lexer_next(&parser->lexer);
  return true;
}

bool parse_machine(const char* file_name, const char* content, machine_t* m) {
  parser_t parser;
  parser.file_name = file_name;
  parser.lexer = lexer_new(content);

  parser.states = set_new();
  parser.symbols = set_new();

  parser.lines = lines_new();

  // Do this so empty symbol will always be 0 no matter when they are
  // encountered in the source file.
  set_find_and_add(&parser.symbols, "_", 1);

  token_t token = lexer_next(&parser.lexer);

  while (token.type != TOKEN_EOF) {
    if (token.type == TOKEN_COMMENT || token.type == TOKEN_NEWLINE) {
      token = lexer_next(&parser.lexer);
      continue;
    }

    if (token.type != TOKEN_IDEN) {
      error_wrong_token(parser.file_name, token, TOKEN_IDEN);
      return false;
    }
    state_t idx = set_find_and_add(&parser.states, token.text, token.length);

    if (!expect_token(&parser, &token, TOKEN_LCURLY)) return false;
    if (!parse_block(&parser, idx)) return false;

    token = lexer_next(&parser.lexer);
  }

  *m = machine_new(parser.symbols, parser.states);

  for (size_t i = 0; i < parser.lines.length; ++i) {
    line_t l = parser.lines.data[i];
    machine_add_rule(m, l.current, l.read, l.write, l.dir, l.next);
  }

  set_free(&parser.states);
  set_free(&parser.symbols);
  lines_free(&parser.lines);

  return true;
}
