#ifndef TAPE_H
#define TAPE_H

#include <stddef.h>
#include <stdint.h>

typedef uint16_t symbol_t;

typedef struct {
  symbol_t *data;
  size_t size;
  size_t head;
  size_t begin;
  size_t end;
} tape_t;

extern tape_t tape_new(void);
extern void tape_move_left(tape_t *);
extern void tape_move_right(tape_t *);
extern symbol_t tape_read(tape_t *);
extern void tape_write(tape_t *, symbol_t);
extern void tape_free(tape_t *);

#endif
