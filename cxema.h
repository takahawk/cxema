#ifndef CXEMA_H_
#define CXEMA_H_
typedef struct SValue SValue;
typedef struct Env Env;

typedef struct Cxema Cxema;

struct Cxema {
  Env *genv;

	SValue* (*parse)     (Cxema *self, char *code);
	SValue* (*eval)      (Cxema *self, Env *env, SValue *svalue);
	SValue* (*interpret) (Cxema *self, char *code);

	void    (*release)   (Cxema **pself);
};

struct _CxemaStatic {
	Cxema prototype;

	Cxema* (*form) (void);
};
extern const struct _CxemaStatic CXEMA;

#endif
