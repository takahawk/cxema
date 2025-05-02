#ifndef CXEMA_CONS_
#define CXEMA_CONS_
#include <stdbool.h>
#include <stddef.h>

#include "svalue.h"

typedef struct Env Env;

struct _ConsStatic {
  SValue* (*car)     (SValue *sval);
  SValue* (*cdr)     (SValue *sval);
  SValue* (*cdar)    (SValue *sval);
  SValue* (*cddar)   (SValue *sval);
  bool    (*is_list) (SValue *sval);

  struct {
    // release only cons cell envelopes, retaining contents
    void    (*release_envelope) (SValue **sval);
    size_t  (*len)              (SValue *sval);
    bool    (*is_all)           (SValue *sval, bool (*cb) (SValue *val));
    SValue* (*reverse)          (SValue *list);
    void    (*eval_items)       (SValue *list, Env *env);
    void    (*println_items)    (SValue *list);

    // take first value, release everything else
    SValue* (*take_first)       (SValue *list);
    // take last value, release everything else
    SValue* (*take_last)        (SValue *list);
  } list;
};

extern const struct _ConsStatic CONS;

#endif // !CXEMA_CONS_

