#include "runner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../common.h"
#include "../core/machine.h"
#include "../platform/io.h"
#include "tape.h"

static void machine_run(machine_t* m, const char* input) {
  tape_t tape = tape_new();

  const char* ptr = input;

  sized_str_t* symbols = malloc(m->symbol_count * sizeof(sized_str_t));
  char* symbol_ptr = (char*)m->symbols.data;

  for (size_t i = 0; i < m->symbol_count; ++i) {
    symbols[i].data = symbol_ptr;
    symbols[i].len = strlen(symbol_ptr);
    symbol_ptr += symbols[i].len + 1;
  }

  // Parse the input string. Essentially this is another lexer.
  while (*ptr != 0) {
    while (*ptr == ' ') ptr++;
    if (*ptr == 0) break;
    const char* next = ptr + 1;

    while (*next != 0 && *next != ' ') next++;
    size_t len = next - ptr;

    bool found = false;
    for (size_t i = 0; i < m->symbol_count; ++i) {
      if (len != symbols[i].len) continue;
      if (memcmp(ptr, symbols[i].data, len)) continue;

      found = true;
      tape_write(&tape, i);
      tape_move_right(&tape);
      break;
    }

    if (!found) {
      fprintf(stderr, "ERROR: Unknown symbol: %.*s\n", (int)len, ptr);
      return;
    }
    ptr = next;
  }

  tape.head = tape.begin;
  uint16_t current_state = 1;

  for (;;) {
    size_t state_idx = m->symbol_count * (current_state - 1);
    rule_t* rule = m->rules + state_idx + tape_read(&tape);

    // The preserved 0 mentioned earlier
    if (rule->next_state == 0) break;

    tape_write(&tape, rule->write_symbol);

    switch (rule->move_dir) {
      case HOLD: break;
      case LEFT: tape_move_left(&tape); break;
      case RIGHT: tape_move_right(&tape); break;
    }

    current_state = rule->next_state;
  }

  printf("OUTPUT: ");
  for (size_t i = tape.begin; i < tape.end; ++i) {
    printf("%s ", symbols[tape.data[i]].data);
  }

  const char* state_ptr = m->states.data;
  while (current_state > 1) {
    state_ptr += strlen(state_ptr) + 1;
    current_state--;
  }
  printf("\n -- %s --\n", state_ptr);

  free(symbols);
  tape_free(&tape);
}

int run(const char* file_path) {
  sized_str_t content;
  char* input = NULL;
  if (!read_file(file_path, &content)) goto fail;
  machine_t m;

  if (!load_machine(content, &m)) {
    fprintf(stderr, "Can't parse machine file `%s`\n", file_path);
    goto fail;
  }

  printf("INPUT:  ");
  input = read_line();
  machine_run(&m, input);

  free(input);
  free(content.data);
  return 0;
fail:
  if (content.data) free(content.data);
  if (input) free(input);
  return 1;
}
