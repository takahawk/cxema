#ifndef CXEMA_CONS_
#define CXEMA_CONS_
#include <stdbool.h>
#include <stddef.h>

#include "svalue.h"

struct _ConsStatic {
  SValue* (*car)     (SValue *sval);
  SValue* (*cdr)     (SValue *sval);
  SValue* (*cdar)    (SValue *sval);
  SValue* (*cddar)   (SValue *sval);
  bool    (*is_list) (SValue *sval);

  struct {
    size_t (*len)    (SValue *sval);
    bool   (*is_all) (SValue *sval, bool (*cb) (SValue *val));
  } list;
};

extern const struct _ConsStatic CONS;

#endif // !CXEMA_CONS_

