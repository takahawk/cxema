#include "builtins.h"

#include <stdbool.h>

#include "env.h"
#include "svalue.h"

static bool _is_number(SValue *val)
{
  return val->type == SVAL_TYPE_INT ||
         val->type == SVAL_TYPE_FLOAT;
}

static SValue* _eval_sum(Env *env, SValue *args, void *ctx)
{
  if (args && args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  double fsum = 0.;
  int64_t isum = 0;
  bool is_float = false;

  while (args) {
    SValue *car = args->val.cons.car;
    SValue *cdr = args->val.cons.cdr;

    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                           SVALUE.to_string(car),
                           SVALUE_TYPE.to_string(car->type));
    }

    if (is_float) {
      fsum += car->type == SVAL_TYPE_INT
        ? car->val._int
        : car->val._float;
    } else {
      if (car->type == SVAL_TYPE_FLOAT) {
        fsum = isum;
        is_float = true;
        continue;
      }
      isum += car->val._int;
    }
    args = cdr;
  }

  return is_float
    ? SVALUE._float(fsum)
    : SVALUE._int(isum);
}

static void define_all(Env *env)
{
  env->set(env, "+", SVALUE.func(_eval_sum, NULL));
}

const struct _BuiltinsStatic BUILTIN = {
  .define_all = define_all,
};
