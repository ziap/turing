#include "tape.h"

#include <stdlib.h>
#include <string.h>

#define TAPE_INIT_SIZE 256  // * 4 bytes = 1 KB

void tape_init(tape_t *tape) {
  tape->size = TAPE_INIT_SIZE;
  tape->data = malloc(tape->size * sizeof(symbol_t));
  memset(tape->data, 0, tape->size * sizeof(symbol_t));

  tape->head = tape->size / 2;
  tape->begin = tape->head;
  tape->end = tape->head + 1;
}

void tape_move_right(tape_t *tape) {
  if (tape->head + 1 == tape->size) {
    symbol_t *new_data = malloc(tape->size * 2 * sizeof(symbol_t));

    memset(new_data + tape->size, 0, tape->size * sizeof(symbol_t));
    memcpy(new_data, tape->data, tape->size * sizeof(symbol_t));

    free(tape->data);
    tape->data = new_data;
    tape->size *= 2;
  }

  tape->head++;
}

void tape_move_left(tape_t *tape) {
  if (tape->head == 0) {
    symbol_t *new_data = malloc(tape->size * 2 * sizeof(symbol_t));
    tape->head += tape->size;
    tape->begin += tape->size;
    tape->end += tape->size;

    memset(new_data, 0, tape->size * sizeof(symbol_t));
    memcpy(new_data + tape->size, tape->data, tape->size * sizeof(symbol_t));

    free(tape->data);
    tape->data = new_data;
    tape->size *= 2;
  }

  tape->head--;
}

symbol_t tape_read(tape_t *tape) { return tape->data[tape->head]; }

void tape_write(tape_t *tape, symbol_t value) {
  if (value) {
    if (tape->head >= tape->end) tape->end = tape->head + 1;
    if (tape->head < tape->begin) tape->begin = tape->head;
  }

  tape->data[tape->head] = value;
}

void tape_free(tape_t *tape) {
  if (tape->data) free(tape->data);
}
