#!/bin/sh

set -xe

CC=clang

CFLAGS="-Wall -Wextra -pedantic -fshort-enums"
# CFLAGS+=" -Og -ggdb"
CFLAGS+=" -O3 -march=native -mtune=native -s"

LDLIBS=""
SRC="$(ls src/*.c)"

$CC -o turing $CFLAGS $LDLIBS $SRC
