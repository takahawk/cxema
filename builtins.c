#include "builtins.h"

#include <stdbool.h>

#include "env.h"
#include "svalue.h"

static bool _is_number(SValue *val)
{
  // TODO: decimal
  return val->type == SVAL_TYPE_NUM;
}

static SValue* _eval_sum(Env *env, SValue *args, void *ctx)
{
  if (args && args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  // TODO: decimal!!
  long sum = 0;

  while (args) {
    SValue *car = args->val.cons.car;
    SValue *cdr = args->val.cons.cdr;

    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                           SVALUE.to_string(car),
                           SVALUE_TYPE.to_string(car->type));
    }

    sum += car->val.num;
    args = cdr;
  }

  return SVALUE.num(sum);
}

static void define_all(Env *env)
{
  env->set(env, "+", SVALUE.func(_eval_sum, NULL));
}

const struct _BuiltinsStatic BUILTIN = {
  .define_all = define_all,
};
