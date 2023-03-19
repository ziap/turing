#include "strarr.h"

#include <stdlib.h>
#include <string.h>

void strarr_write(char **src, size_t size, FILE *f) {
  size_t total_size = src[size - 1] - src[0] + strlen(src[size - 1]);
  fwrite(&total_size, 1, sizeof(size_t), f);
  fwrite(src[0], 1, total_size, f);
}

char **strarr_read(size_t size, FILE *f) {
  size_t total_size;
  fread(&total_size, 1, sizeof(size_t), f);

  char **src = malloc(size * sizeof(char *));
  src[0] = malloc(total_size);
  fread(src[0], 1, total_size, f);

  for (size_t i = 1; i < size; ++i) {
    src[i] = src[i - 1] + strlen(src[i - 1]) + 1;
  }

  return src;
}

char **strarr_from_set(set_t set) {
  size_t size = set.length;
  size_t total_offset = 0;
  for (size_t i = 0; i < size; ++i) { total_offset += set.data[i].length + 1; }

  char **dest = malloc(size * sizeof(char *));
  dest[0] = malloc(total_offset);
  memcpy(dest[0], set.data[0].str, set.data[0].length);
  dest[0][set.data[0].length] = 0;

  for (size_t i = 1; i < size; ++i) {
    dest[i] = dest[i - 1] + set.data[i - 1].length + 1;
    memcpy(dest[i], set.data[i].str, set.data[i].length);
    dest[i][set.data[i].length] = 0;
  }

  return dest;
}

void strarr_free(char **arr) {
  free(arr[0]);
  free(arr);
}
