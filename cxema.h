#ifndef CXEMA_H_
#define CXEMA_H_
#include "svalue.h"

typedef struct Cxema Cxema;

struct Cxema {
	SValue* (*interpret) (Cxema *self, char *code);
	void    (*release)   (Cxema **pself);
};

extern const Cxema CXEMA_PROTOTYPE;
// TODO: custom allocator?
Cxema *form_cxema();

#endif