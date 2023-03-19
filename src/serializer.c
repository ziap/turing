#include "serializer.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "strarr.h"

bool save_machine(machine_t *m, const char *file_path) {
  FILE *f = fopen(file_path, "wb");
  if (f == NULL) return false;

  fwrite(&m->symbol_count, 1, sizeof(symbol_t), f);
  fwrite(&m->state_count, 1, sizeof(state_t), f);

  strarr_write(m->symbols, m->symbol_count, f);
  strarr_write(m->states, m->state_count, f);

  fwrite(m->rules, 1, sizeof(rule_t) * m->state_count * m->symbol_count, f);

  fclose(f);
  return true;
}

bool load_machine(machine_t *m, const char *file_path) {
  FILE *f = fopen(file_path, "rb");
  if (f == NULL) return false;

  fread(&m->symbol_count, 1, sizeof(symbol_t), f);
  fread(&m->state_count, 1, sizeof(state_t), f);

  m->symbols = strarr_read(m->symbol_count, f);
  m->states = strarr_read(m->state_count, f);

  m->rules = malloc(sizeof(rule_t) * m->state_count * m->symbol_count);
  fread(m->rules, 1, sizeof(rule_t) * m->state_count * m->symbol_count, f);

  fclose(f);
  return true;
}
