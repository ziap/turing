#include "lexer.h"

#include <stdlib.h>
#include <string.h>

const char* token_display[TOKEN_COUNT] = {
  "COMMENT", "{", "}", "ARROW", "IDENTIFIER", "NEWLINE", "EOF", "UNKNOWN",
};

lexer_t lexer_new(sized_str_t content) {
  lexer_t lexer = {
    .content = content.data,
    .cursor = content.data,
    .end = content.data + content.len,
    .line_start = content.data,
    .line = 1,
  };

  return lexer;
}

static bool match_char(lexer_t* lex, token_t* tok, token_type_t type, char c) {
  if (*lex->cursor != c) return false;

  lex->cursor++;
  tok->type = type;
  tok->text.len = 1;

  return true;
}

token_t lexer_next(lexer_t* lexer) {
  while (lexer->cursor < lexer->end) {
    if (*lexer->cursor != ' ' && *lexer->cursor != '\t') break;
    lexer->cursor++;
  }

  token_t token;
  token.line = lexer->line;
  token.col = lexer->cursor - lexer->line_start + 1;
  token.text.data = lexer->cursor;

  if (lexer->cursor >= lexer->end) {
    token.type = TOKEN_EOF;
    token.text.len = 0;

    return token;
  };

  if (match_char(lexer, &token, TOKEN_LCURLY, '{')) return token;
  if (match_char(lexer, &token, TOKEN_RCURLY, '}')) return token;
  if (match_char(lexer, &token, TOKEN_ARROW, '<')) return token;
  if (match_char(lexer, &token, TOKEN_ARROW, '>')) return token;
  if (match_char(lexer, &token, TOKEN_ARROW, '-')) return token;

  if (*lexer->cursor == '\n') {
    lexer->cursor++;
    lexer->line++;
    lexer->line_start = lexer->cursor;

    token.type = TOKEN_NEWLINE;
    token.text.len = 1;

    return token;
  }

  if (*lexer->cursor == '#') {
    lexer->cursor++;
    token.type = TOKEN_COMMENT;
    token.text.len = 1;

    // Mark everythin after '#' as comment until a newline character is reached
    while (lexer->cursor < lexer->end && *lexer->cursor != '\n') {
      token.text.len++;
      lexer->cursor++;
    }

    return token;
  }

  // Everything else is an identifier (for now)
  token.type = TOKEN_IDEN;
  token.text.len = 1;
  lexer->cursor++;

  while (lexer->cursor < lexer->end) {
    bool is_iden = true;
    for (const char* i = " \t\n{}<>-#"; *i != 0; ++i) {
      if (*i == *lexer->cursor) {
        is_iden = false;
        break;
      }
    }
    if (!is_iden) break;

    token.text.len++;
    lexer->cursor++;
  }

  return token;
}
