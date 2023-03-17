#ifndef STRARR_H
#define STRARR_H

#include <stdio.h>

extern void strarr_write(char**, size_t, FILE*);
extern char** strarr_read(size_t, FILE*);
extern char** strarr_copy(char**, size_t);

#endif
