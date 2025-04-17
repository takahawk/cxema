#ifndef CXEMA_H_
#define CXEMA_H_
#include "svalue.h"

typedef struct Cxema Cxema;

struct Cxema {
  Env *genv;

	SValue* (*parse)     (Cxema *self, char *code);
	SValue* (*eval)      (Cxema *self, SValue *svalue);
	SValue* (*interpret) (Cxema *self, char *code);

	void    (*release)   (Cxema **pself);
};

struct _CxemaStatic {
	Cxema prototype;

	Cxema* (*form) (void);
};
extern const struct _CxemaStatic CXEMA;

#endif
