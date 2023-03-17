#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "machine.h"
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

int compile(const char* f) {
  (void)f;
  fprintf(stderr, "ERROR: Not implemented\n");
  return 1;
}

int main(int argc, const char** argv) {
  if (argc < 2) {
    fprintf(stderr, "ERROR: Not enough arguments\n");
    printf("run `%s help` for more information\n", argv[0]);
    return 1;
  }

  if (!strcmp(argv[1], "help")) {
    printf("USAGE:\n");
    printf("  %s <command> [options]\n", argv[0]);
    printf("\n");
    printf("COMMANDS:\n");
    printf("  help             -- Show this message\n");
    printf("  run <machine>    -- Run a compiled turing machine\n");
    printf("  compile <source> -- Compile a source file\n");
    return 0;
  }

  if (!strcmp(argv[1], "run")) {
    if (argc < 3) {
      fprintf(stderr, "ERROR: Not enough arguments\n");
      printf("USAGE: %s run <program>\n", argv[0]);
      return 1;
    }
    return run(argv[2]);
  }

  if (!strcmp(argv[1], "compile")) {
    if (argc < 3) {
      fprintf(stderr, "ERROR: Not enough arguments\n");
      printf("USAGE: %s compile <source>\n", argv[0]);
      return 1;
    }
    return compile(argv[2]);
  }

  fprintf(stderr, "ERROR: Unknown command: %s\n", argv[1]);
  return 1;
}
