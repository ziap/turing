#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compiler/compiler.h"
#include "runtime/runner.h"

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
    printf("  help                -- Show this message\n");
    printf("  run <machine>       -- Run a compiled turing machine\n");
    printf("  compile <src> <out> -- Compile a source file\n");
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
    if (argc < 4) {
      fprintf(stderr, "ERROR: Not enough arguments\n");
      printf("USAGE: %s compile <src> <out>\n", argv[0]);
      return 1;
    }
    return compile(argv[2], argv[3]);
  }

  fprintf(stderr, "ERROR: Unknown command: %s\n", argv[1]);
  return 1;
}
