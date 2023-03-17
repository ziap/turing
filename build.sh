#!/bin/sh

set -xe

CC="${CC:-clang}"

CFLAGS="-std=c99 -Wall -Wextra -pedantic -fshort-enums"
# CFLAGS+=" -Og -ggdb"
CFLAGS+=" -O3 -march=native -mtune=native -s"

LDLIBS=""
SRC="$(ls src/*.c)"

$CC -o turing $CFLAGS $LDLIBS $SRC
