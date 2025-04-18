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

static SValue* _eval_sub(Env *env, SValue *args, void *ctx)
{
  if (args && args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  SValue *car = args->val.cons.car;
  SValue *cdr = args->val.cons.cdr;
  if (!car) {
    return SVALUE.errorf("at least one argument is expected for -");
  }
  if (!_is_number(car)) {
    return SVALUE.errorf("invalid number: %s (type=%s)",
                         SVALUE.to_string(car),
                         SVALUE_TYPE.to_string(car->type));
  }
  if (!cdr) {
    // single argument
    return car->type == SVAL_TYPE_INT
      ? SVALUE._int(-car->val._int)
      : SVALUE._float(-car->val._float);
  }

  bool is_float = car->type == SVAL_TYPE_FLOAT;
  double fres = is_float ? car->val._float : 0.;
  int64_t ires = is_float ? 0 : car->val._int;
  args = cdr;

  while (args) {
    car = args->val.cons.car;
    cdr = args->val.cons.cdr;

    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                           SVALUE.to_string(car),
                           SVALUE_TYPE.to_string(car->type));
    }

    if (is_float) {
      fres -= car->type == SVAL_TYPE_INT
        ? car->val._int
        : car->val._float;
    } else {
      if (car->type == SVAL_TYPE_FLOAT) {
        fres = ires;
        is_float = true;
        continue;
      }
      ires -= car->val._int;
    }
    args = cdr;
  }

  return is_float
    ? SVALUE._float(fres)
    : SVALUE._int(ires);
}

static void define_all(Env *env)
{
  env->set(env, "+", SVALUE.func(_eval_sum, NULL));
  env->set(env, "-", SVALUE.func(_eval_sub, NULL));
}

const struct _BuiltinsStatic BUILTIN = {
  .define_all = define_all,
};
