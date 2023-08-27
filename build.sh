#!/bin/sh -xe

CC="${CC:-clang}"

CFLAGS="-std=c99 -Wall -Wextra -pedantic -fshort-enums"
LDLIBS=""

BUILD_FLAGS="-O3 -march=native -mtune=native -s"
DEBUG_FLAGS="-Og -ggdb"

NATIVE_MODULES="core platform runtime compiler"
SRC="src/main.c"

for MODULE in $NATIVE_MODULES
do
  SRC+=" $(ls src/$MODULE/*.c)"
done

# Compile the executable
$CC -o turing $CFLAGS $BUILD_FLAGS $LDLIBS $SRC

# Compile the examples
for FILE in $(ls examples/*.tr)
do
  OUTPUT=$(basename $FILE)
  ./turing compile $FILE compiled/${OUTPUT/\.tr/\.tb}
done
