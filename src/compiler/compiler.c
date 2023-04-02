#include "compiler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/machine.h"
#include "../platform/io.h"
#include "parser.h"

int compile(const char* in, const char* out) {
  sized_str_t content;
  parser_t parser = {0};
  rule_t* rules = NULL;
  FILE* f = NULL;

  if (!read_file(in, &content)) goto fail;

  parser = parser_new(in, content);
  if (!parse_machine(&parser)) goto fail;

  uint16_t symbol_count = parser.symbols.count;
  uint16_t state_count = parser.states.count;

  rules = malloc(symbol_count * state_count * sizeof(rule_t));
  memset(rules, 0, symbol_count * state_count * sizeof(rule_t));
  for (size_t i = 0; i < parser.lines.length; ++i) {
    line_t line = parser.lines.data[i];
    rule_t* rule = rules + (symbol_count * line.current + line.read);

    rule->write_symbol = line.write;
    rule->move_dir = line.dir;

    // Preserve 0 for explicit halt state
    rule->next_state = line.next + 1;
  }

  f = fopen(out, "wb");
  if (!f) goto fail;

  uint32_t len = parser.symbols.size;
  fwrite(&len, 1, sizeof(len), f);
  fwrite(parser.symbols.data, 1, len, f);

  len = parser.states.size;
  fwrite(&len, 1, sizeof(len), f);
  fwrite(parser.states.data, 1, len, f);

  fwrite(rules, 1, symbol_count * state_count * sizeof(rule_t), f);

  free(content.data);
  parser_cleanup(&parser);
  free(rules);
  fclose(f);
  return 0;
fail:
  if (errno) {
    fprintf(
      stderr, "ERROR: failed to write file `%s`: %s\n", out, strerror(errno)
    );
  }

  if (content.data) free(content.data);
  parser_cleanup(&parser);
  if (rules) free(rules);
  if (f) fclose(f);
  return 1;
}
