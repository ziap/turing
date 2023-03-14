#!/bin/sh

set -xe

CC=clang
CFLAGS="-Wall -Wextra -std=c99 -pedantic -O3 -march=native -mtune=native"
LDLIBS=""
SRC="$(ls src/*.c) main.c"

$CC -o main $CFLAGS $LDLIBS $SRC
