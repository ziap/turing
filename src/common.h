#ifndef COMMON_H
#define COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define memcpy __builtin_memcpy
#define memset __builtin_memset
#define memcmp __builtin_memcmp

typedef struct {
  char *data;

  // 32-bit is enough for string length
  uint32_t len;
} sized_str_t;

typedef enum {
  HOLD = 0,
  LEFT = -1,
  RIGHT = 1,
} move_t;

#endif
