#include "dumbset.h"

#include <stdlib.h>
#include <string.h>

#define SET_INIT_CAPACITY 256
set_t set_new(void) {
  return (set_t){
    .data = malloc(SET_INIT_CAPACITY),
    .size = 0,
    .count = 0,
    .capacity = SET_INIT_CAPACITY,
  };
}

size_t set_find_and_add(set_t *set, sized_str_t str) {
  size_t idx = 0;
  size_t str_idx = 0;

  while (str_idx < set->size) {
    size_t len = strlen(set->data + str_idx);
    if (len == str.len) {
      if (!memcmp(set->data + str_idx, str.data, len)) return idx;
    }
    str_idx += len + 1;
    idx++;
  }

  if (set->size + str.len + 1 > set->capacity) {
    while (set->size + str.len + 1 > set->capacity) set->capacity *= 2;
    set->data = realloc(set->data, set->capacity);
  }

  memcpy(set->data + set->size, str.data, str.len);
  set->data[set->size + str.len] = 0;
  set->size += str.len + 1;
  set->count = idx + 1;

  return idx;
}

void set_free(set_t *set) {
  if (set->data) free(set->data);
}
