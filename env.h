#ifndef CXEMA_ENV_H_
#define CXEMA_ENV_H_
#include "codex/ds/array.h"

struct SValue;
typedef struct SValue SValue;

typedef struct Env Env;

struct Env {
  Env *parent;
  // TODO: optimize? radix tree, hashtable, balanced btree whatever
  Array*/*char**/ symbols;
  Array*/*SValue**/ values;

  void    (*set)       (Env *self, char *symbol, SValue *val);
  void    (*setnocopy) (Env *self, char *symbol, SValue *val);
  SValue* (*get)       (Env *self, char *symbol);

  void    (*release) (Env **pself);
};

struct _EnvStatic {
  Env  prototype;

  Env* (*form) (void);
  Env* (*copy) (Env *original);
};

extern const struct _EnvStatic ENV;

#endif
