#include "tape.h"

#include <stdlib.h>

#define TAPE_INIT_SIZE 512  // * 2 bytes = 1 KB

tape_t tape_new(void) {
  tape_t tape;
  tape.size = TAPE_INIT_SIZE;
  tape.data = malloc(tape.size * sizeof(uint16_t));
  memset(tape.data, 0, tape.size * sizeof(uint16_t));

  tape.head = tape.size / 2;
  tape.begin = tape.head;
  tape.end = tape.head + 1;
  return tape;
}

void tape_move_right(tape_t *tape) {
  if (tape->head + 1 == tape->size) {
    uint16_t *new_data = malloc(tape->size * 2 * sizeof(uint16_t));

    memset(new_data + tape->size, 0, tape->size * sizeof(uint16_t));
    memcpy(new_data, tape->data, tape->size * sizeof(uint16_t));

    free(tape->data);
    tape->data = new_data;
    tape->size *= 2;
  }

  tape->head++;
}

void tape_move_left(tape_t *tape) {
  if (tape->head == 0) {
    uint16_t *new_data = malloc(tape->size * 2 * sizeof(uint16_t));

    // The indices need to point to the second half of the buffer
    tape->head += tape->size;
    tape->begin += tape->size;
    tape->end += tape->size;

    memset(new_data, 0, tape->size * sizeof(uint16_t));
    memcpy(new_data + tape->size, tape->data, tape->size * sizeof(uint16_t));

    free(tape->data);
    tape->data = new_data;
    tape->size *= 2;
  }

  tape->head--;
}

uint16_t tape_read(tape_t *tape) { return tape->data[tape->head]; }

void tape_write(tape_t *tape, uint16_t value) {
  tape->data[tape->head] = value;

  if (value) {
    if (tape->head + 1 > tape->end) tape->end = tape->head + 1;
    if (tape->head < tape->begin) tape->begin = tape->head;
  }

  // Trim extra blank symbols from both sides
  while (tape->end > tape->begin && !tape->data[tape->end - 1]) tape->end--;
  while (tape->end > tape->begin && !tape->data[tape->begin]) tape->begin++;
}

void tape_free(tape_t *tape) { free(tape->data); }
