#include "io.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool read_file(const char *file_path, sized_str_t *content) {
  char *buf = NULL;
  FILE *f = fopen(file_path, "rb");

  if (!f) goto fail;

  if (fseek(f, 0, SEEK_END) < 0) goto fail;

  size_t len = ftell(f);
  if (len < 0) goto fail;

  if (fseek(f, 0, SEEK_SET) < 0) goto fail;

  buf = malloc(len + 1);
  if (!buf) goto fail;

  fread(buf, 1, len, f);
  if (ferror(f)) goto fail;

  buf[len] = 0;
  fclose(f);

  content->data = buf;
  content->len = len;

  return true;
fail:
  if (errno) {
    fprintf(
      stderr, "ERROR: failed to read file `%s`: %s\n", file_path,
      strerror(errno)
    );
  }

  if (f) fclose(f);
  if (buf) free(buf);
  return false;
}

#define INIT_READ_SZ 64
char *read_line(void) {
  size_t sz = INIT_READ_SZ;

  char *buf = malloc(sz);
  fgets(buf, sz, stdin);
  size_t len = strlen(buf);

  while (buf[len - 1] != '\n') {
    sz *= 2;
    buf = realloc(buf, sz);
    fgets(buf + len, sz - len, stdin);
    len += strlen(buf + len);
  }

  buf[len - 1] = 0;
  return buf;
}
