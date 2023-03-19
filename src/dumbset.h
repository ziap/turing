#ifndef DUMBSET_H
#define DUMBSET_H

#include <stddef.h>

typedef struct {
  const char* str;
  size_t length;
} sized_str;

typedef struct {
  sized_str* data;
  size_t length;
  size_t capacity;
} set_t;

extern set_t set_new(void);
extern size_t set_find_and_add(set_t*, const char*, size_t);
extern void set_free(set_t*);

#endif
