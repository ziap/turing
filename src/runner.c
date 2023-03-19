#include "runner.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "serializer.h"

#define INIT_READ_SZ 64
static char* read_line(void) {
  size_t sz = INIT_READ_SZ;

  char* buf = malloc(sz);
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

int run(const char* f) {
  machine_t m;
  if (!load_machine(&m, f)) {
    fprintf(stderr, "ERROR: Can't open file %s: %s\n", f, strerror(errno));
    return 1;
  }

  printf("INPUT:  ");
  char* input = read_line();

  machine_run(&m, input);
  machine_free(&m);

  free(input);
  return 0;
}
