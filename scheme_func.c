#include "scheme_func.h"

#include <stddef.h>

#include "cxema.h"

typedef struct SchemeFuncCtx {
  SValue *args;
  size_t arg_count;
  SValue *body;
  // TODO: evaluator
} SchemeFuncCtx;

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
