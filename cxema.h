#ifndef CXEMA_H_
#define CXEMA_H_
#include "svalue.h"

typedef struct Cxema Cxema;

struct Cxema {
	SValue* (*interpret) (Cxema *self, char *code);
	void    (*release)   (Cxema **pself);
};

struct _CxemaStatic {
	Cxema prototype;
};
extern const struct _CxemaStatic CXEMA;
// TODO: custom allocator?
Cxema *form_cxema();

#endif