#ifndef CXEMA_ENV_H_
#define CXEMA_ENV_H_
#include "codex/ds/array.h"

#include "codex/mem/rc.h"

struct SValue;
typedef struct SValue SValue;

typedef struct Env Env;

struct Env {
  Rc* /*Env**/       rcparent;
  // TODO: optimize? radix tree, hashtable, balanced btree whatever
  Array* /*char**/   symbols;
  Array* /*SValue**/ values;

};

struct _EnvStatic {
  Env  prototype;

  Rc* /*Env**/ (*form)       (void);
  Rc* /*Env**/ (*form_child) (Rc* /*Env**/ rcparent);

  Rc* /*Env**/ (*get_parent) (Rc* /*Env**/ rc_env);

  void    (*set)       (Rc* /*Env**/ rcself, char *symbol, SValue *val);
  void    (*setnocopy) (Rc* /*Env**/ rcself, char *symbol, SValue *val);
  SValue* (*get)       (Rc* /*Env**/ rcself, char *symbol);

  void    (*release)   (Rc* /*Env**/ *pself);
};

extern const struct _EnvStatic ENV;

#endif
