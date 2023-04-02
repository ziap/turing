#include "machine.h"

typedef struct {
  sized_str_t src;

  size_t cur;
} stream_t;

static stream_t stream_new(sized_str_t str) {
  return (stream_t){.src = str, .cur = 0};
}

static bool read(void *dest, size_t size, stream_t *reader) {
  if (reader->cur + size > reader->src.len) return false;

  memcpy(dest, reader->src.data + reader->cur, size);
  reader->cur += size;

  return true;
}

static bool reference(void **dest, size_t size, stream_t *reader) {
  if (reader->cur + size > reader->src.len) return false;

  *dest = reader->src.data + reader->cur;
  reader->cur += size;

  return true;
}

bool load_machine(sized_str_t str, machine_t *m) {
  stream_t reader = stream_new(str);
  if (!read(&m->symbols.len, sizeof(m->symbols.len), &reader)) return false;
  if (!reference((void **)&m->symbols.data, m->symbols.len, &reader)) {
    return false;
  }

  if (!read(&m->states.len, sizeof(m->states.len), &reader)) return false;
  if (!reference((void **)&m->states.data, m->states.len, &reader)) {
    return false;
  }

  m->symbol_count = 0;
  for (size_t i = 0; i < m->symbols.len; ++i) {
    if (m->symbols.data[i] == '_') m->symbols.data[i] = ' ';
    if (m->symbols.data[i] == 0) m->symbol_count++;
  }

  m->state_count = 0;
  for (size_t i = 0; i < m->states.len; ++i) {
    if (m->states.data[i] == '_') m->states.data[i] = ' ';
    if (m->states.data[i] == 0) m->state_count++;
  }

  size_t rules_len = (m->symbol_count * m->state_count) * sizeof(rule_t);
  if (!reference((void **)&m->rules, rules_len, &reader)) return false;

  return true;
}
