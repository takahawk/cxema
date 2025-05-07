#ifndef CXEMA_H_
#define CXEMA_H_
typedef struct SValue SValue;
typedef struct Env Env;

typedef struct Cxema Cxema;

struct Cxema {
  Env *genv;

	SValue* (*parse)         (Cxema *self, char *code);
	SValue* (*eval)          (Cxema *self, SValue *svalue);

  // parses and evaluates all, returns the last value
	SValue* (*interpret)     (Cxema *self, char *code);

  // parses and evaluates all, returns cons of results
  SValue* (*interpret_all) (Cxema *self, char *code);

  SValue* (*interpret_file)     (Cxema *self, char *filename);
  SValue* (*interpret_file_all) (Cxema *self, char *filename);

	void    (*release)   (Cxema **pself);
};

struct _CxemaStatic {
	Cxema prototype;

	Cxema* (*form) (void);
};
extern const struct _CxemaStatic CXEMA;

#endif
