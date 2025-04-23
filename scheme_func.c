#include "cxema.h"
#include "scheme_func.h"

static SValue* form(SValue *args, SValue *body)
{
  if (args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("cons expected, got %s", args->type);
  }

  return SVALUE.errorf("not implmented");
}

const struct _SchemeFuncStatic SCHEME_FUNC = {
  .form = form,
};
