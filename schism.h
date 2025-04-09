#ifndef SCHISM_H_
#define SCHISM_H_
#include "svalue.h"

typedef struct Schism Schism;

struct Schism {
	SValue* (*interpret) (Schism *self, char *code);
};

extern const Schism SCHISM_PROTOTYPE;
// TODO: custom allocator?
Schism *form_schism();

#endif