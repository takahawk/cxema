#ifndef CXEMA_BUILTINS_H_
#define CXEMA_BUILTINS_H_
#include "codex/mem/rc.h"

struct Env;
typedef struct Env Env;

struct _BuiltinsStatic {
  void (*define_all) (Rc* /*Env**/ env);
};

extern const struct _BuiltinsStatic BUILTIN;
#endif
