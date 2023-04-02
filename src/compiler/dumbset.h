#ifndef DUMBSET_H
#define DUMBSET_H

#include "../common.h"

typedef struct {
  char* data;
  size_t size;
  size_t count;
  size_t capacity;
} set_t;

extern set_t set_new(void);

// O(N) time complexity for finding an element, that's why this set is dumb
extern size_t set_find_and_add(set_t*, sized_str_t);

extern void set_free(set_t*);

#endif
