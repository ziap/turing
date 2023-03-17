#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <stdbool.h>

#include "machine.h"

extern bool load_machine(machine_t*, const char*);
extern bool save_machine(machine_t*, const char*);

#endif
