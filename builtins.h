#ifndef CXEMA_BUILTINS_H_
#define CXEMA_BUILTINS_H_
struct Env;
typedef struct Env Env;

struct _BuiltinsStatic {
  void (*define_all) (Env *env);
};

extern const struct _BuiltinsStatic BUILTIN;
#endif
