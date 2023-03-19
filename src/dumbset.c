#include "dumbset.h"

#include <stdlib.h>
#include <string.h>

#define SET_INIT_CAP 16
set_t set_new(void) {
  set_t set = {
    .data = malloc(SET_INIT_CAP * sizeof(sized_str)),
    .length = 0,
    .capacity = SET_INIT_CAP,
  };
  return set;
}

size_t set_find_and_add(set_t *set, const char *str, size_t len) {
  for (size_t i = 0; i < set->length; ++i) {
    if (len != set->data[i].length) continue;
    if (memcmp(str, set->data[i].str, len)) continue;
    return i;
  }

  if (set->length >= set->capacity) {
    set->capacity *= 2;
    set->data = realloc(set->data, set->capacity * sizeof(sized_str));
  }

  set->length++;
  set->data[set->length - 1].length = len;
  set->data[set->length - 1].str = str;

  return set->length - 1;
}

void set_free(set_t *set) { free(set->data); }
