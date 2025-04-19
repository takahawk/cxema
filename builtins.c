#include "builtins.h"

#include <stdbool.h>

#include "env.h"
#include "svalue.h"

static inline bool _is_number(SValue *val)
{
  return val->type == SVAL_TYPE_INT ||
         val->type == SVAL_TYPE_FLOAT;
}

static inline bool _is_zero(SValue *val)
{
  return (val->type == SVAL_TYPE_INT && val->val._int == 0) ||
         (val->type == SVAL_TYPE_FLOAT && val->val._float == 0);
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
  if (!args) {
    return SVALUE.errorf("at least one argument is expected for -");
  }
  if (args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  SValue *car = args->val.cons.car;
  SValue *cdr = args->val.cons.cdr;
  bool is_float;
  double fres;
  int64_t ires;
  if (cdr) {
    // >= 2 args
    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                          SVALUE.to_string(car),
                          SVALUE_TYPE.to_string(car->type));
    }
    is_float = car->type == SVAL_TYPE_FLOAT;
    fres = is_float ? car->val._float : 0.;
    ires = is_float ? 0 : car->val._int;
    args = cdr;
  } else {
    // single argument
    is_float = false;
    fres = 0.;
    ires = 0;
  }


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

static SValue* _eval_mul(Env *env, SValue *args, void *ctx)
{
  if (args && args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  double fprod = 1.;
  int64_t iprod = 1;
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
      fprod *= car->type == SVAL_TYPE_INT
        ? car->val._int
        : car->val._float;
    } else {
      if (car->type == SVAL_TYPE_FLOAT) {
        fprod = iprod;
        is_float = true;
        continue;
      }
      iprod *= car->val._int;
    }
    args = cdr;
  }

  return is_float
    ? SVALUE._float(fprod)
    : SVALUE._int(iprod);
}

static SValue* _eval_div(Env *env, SValue *args, void *ctx)
{
  if (!args) {
    return SVALUE.errorf("at least one argument is expected for /");
  }
  if (args && args->type != SVAL_TYPE_CONS) {
    return SVALUE.errorf("arguments expected to be cons cell. Actual type: %s",
                         SVALUE_TYPE.to_string(args->type));
  }

  SValue *car = args->val.cons.car;
  SValue *cdr = args->val.cons.cdr;

  bool is_float;
  double fres;
  int64_t ires;

  if (cdr) {
    // >= 2 args, start with first number
    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                           SVALUE.to_string(car),
                           SVALUE_TYPE.to_string(car->type));
    }

    is_float = car->type == SVAL_TYPE_FLOAT;
    fres = is_float ? car->val._float : 1.;
    ires = is_float ? 1 : car->val._int;
    args = cdr;
  } else {
    // 1 arg - start with 1
    is_float = false;
    fres = 1.;
    ires = 1;
  }

  while (args) {
    car = args->val.cons.car;
    cdr = args->val.cons.cdr;

    if (!_is_number(car)) {
      return SVALUE.errorf("invalid number: %s (type=%s)",
                           SVALUE.to_string(car),
                           SVALUE_TYPE.to_string(car->type));
    }

    if (_is_zero(car))
      return SVALUE.errorf("Division by Zero");

    if (is_float) {
      fres /= car->type == SVAL_TYPE_INT
        ? car->val._int
        : car->val._float;
    } else {
      if (car->type == SVAL_TYPE_FLOAT ||
          ires % car->val._int != 0) {
        fres = ires;
        is_float = true;
        continue;
      }
      ires /= car->val._int;
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
  env->set(env, "*", SVALUE.func(_eval_mul, NULL));
  env->set(env, "/", SVALUE.func(_eval_div, NULL));
}

const struct _BuiltinsStatic BUILTIN = {
  .define_all = define_all,
};
