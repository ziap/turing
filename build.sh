#!/bin/sh

set -xe

CC=clang
CFLAGS="-Wall -Wextra -pedantic -fshort-enums"
# CFLAGS+=" -ggdb"
CFLAGS+=" -O3 -march=native -mtune=native -s"

LDLIBS=""
SRC="$(ls src/*.c) main.c"

$CC -o turing $CFLAGS $LDLIBS $SRC
