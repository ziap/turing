#include "compiler.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"
#include "parser.h"
#include "serializer.h"

char* slurp_file(const char* file_name) {
  FILE* f = fopen(file_name, "rb");
  if (!f) return NULL;

  fseek(f, 0, SEEK_END);
  size_t len = ftell(f);
  fseek(f, 0, SEEK_SET);

  char* buf = malloc(len + 1);
  fread(buf, 1, len, f);

  buf[len] = 0;

  fclose(f);

  return buf;

  // TODO: Handle errors for fseek, ftell, ...
}

int compile(const char* in, const char* out) {
  char* content = slurp_file(in);
  if (!content) {
    fprintf(stderr, "ERROR: Can't open file %s: %s\n", in, strerror(errno));
    return 1;
  }

  machine_t m;
  if (!parse_machine(in, content, &m)) return 1;
  save_machine(&m, out);

  machine_free(&m);
  free(content);
  return 0;
}
