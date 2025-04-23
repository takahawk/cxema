#ifndef CXEMA_SCHEME_FUNC_
#define CXEMA_SCHEME_FUNC_

#include "svalue.h"

struct _SchemeFuncStatic {
  SValue* (*form) (SValue *args, SValue* body);
};

extern const struct _SchemeFuncStatic SCHEME_FUNC;

#endif // !CXEMA_SCHEME_FUNC_
