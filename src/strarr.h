#ifndef STRARR_H
#define STRARR_H

#include <stdio.h>

#include "dumbset.h"

extern void strarr_write(char**, size_t, FILE*);
extern char** strarr_read(size_t, FILE*);
extern char** strarr_from_set(set_t);
extern void strarr_free(char**);

#endif
