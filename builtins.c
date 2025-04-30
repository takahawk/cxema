#include "builtins.h"

#include <stdbool.h>

#include "cons.h"
#include "env.h"
#include "svalue.h"

static inline bool _is_zero(SValue *val)
{
  return (val->type == SVAL_TYPE_INT && val->val._int == 0) ||
         (val->type == SVAL_TYPE_FLOAT && val->val._float == 0);
}

static SValue* _eval_sum(SValue *args)
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

    if (!SVALUE.is_number(car)) {
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

static SValue* _eval_sub(SValue *args)
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
    if (!SVALUE.is_number(car)) {
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

    if (!SVALUE.is_number(car)) {
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

static SValue* _eval_mul(SValue *args)
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

    if (!SVALUE.is_number(car)) {
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

static SValue* _eval_div(SValue *args)
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
    if (!SVALUE.is_number(car)) {
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

    if (!SVALUE.is_number(car)) {
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

static SValue* _cmp_oper(SValue *args, 
                         bool (*cmp_int)   (int64_t a, int64_t b),
                         bool (*cmp_float) (double a, double b))
{
  if (CONS.list.len(args) < 2) {
    return SVALUE._bool(true);
  }

  while (args->val.cons.cdr) {
    SValue *cdr = args->val.cons.cdr;
    SValue *a = args->val.cons.car;
    SValue *b = cdr->val.cons.car;

    if (!SVALUE.is_number(a)) {
      return SVALUE.typeerr(a, SVAL_TYPE_NUMBER);
    }
    if (!SVALUE.is_number(b)) {
      return SVALUE.typeerr(b, SVAL_TYPE_NUMBER);
    }

    if (a->type == SVAL_TYPE_INT && b->type == SVAL_TYPE_INT) {
      if (!cmp_int(a->val._int, b->val._int))
        return SVALUE._bool(false);
    } else {
      double aval = a->type == SVAL_TYPE_INT ? a->val._int : a->val._float;
      double bval = b->type == SVAL_TYPE_INT ? b->val._int : b->val._float;
      if (!cmp_float(aval, bval))
        return SVALUE._bool(false);
    }

    args = cdr;
  }

  return SVALUE._bool(true);
}

static bool _eq_int(int64_t a, int64_t b) { return a == b; }
static bool _eq_float(double a, double b) { return a == b; }
static SValue* _eval_eq(SValue* args) { return _cmp_oper(args, _eq_int, _eq_float); }

static bool _gt_int(int64_t a, int64_t b) { return a > b; }
static bool _gt_float(double a, double b) { return a > b; }
static SValue* _eval_gt(SValue* args) { return _cmp_oper(args, _gt_int, _gt_float); }


static bool _ge_int(int64_t a, int64_t b) { return a >= b; }
static bool _ge_float(double a, double b) { return a >= b; }
static SValue* _eval_ge(SValue* args) { return _cmp_oper(args, _ge_int, _ge_float); }

static bool _lt_int(int64_t a, int64_t b) { return a < b; }
static bool _lt_float(double a, double b) { return a < b; }
static SValue* _eval_lt(SValue* args) { return _cmp_oper(args, _lt_int, _lt_float); }


static bool _le_int(int64_t a, int64_t b) { return a <= b; }
static bool _le_float(double a, double b) { return a <= b; }
static SValue* _eval_le(SValue* args) { return _cmp_oper(args, _le_int, _le_float); }

static SValue *_eval_not(SValue *args)
{
  if (CONS.list.len(args) != 1) {
    return SVALUE.errorf("`not` is expecting exactly one argument");
  }

  if (SVALUE.is_false(CONS.car(args)))
    return SVALUE._bool(true);

  return SVALUE._bool(false);
}

static void define_all(Env *env)
{
  env->setnocopy(env, "+", SVALUE.builtin_func(_eval_sum));
  env->setnocopy(env, "-", SVALUE.builtin_func(_eval_sub));
  env->setnocopy(env, "*", SVALUE.builtin_func(_eval_mul));
  env->setnocopy(env, "/", SVALUE.builtin_func(_eval_div));

  env->setnocopy(env, "=", SVALUE.builtin_func(_eval_eq));
  env->setnocopy(env, ">", SVALUE.builtin_func(_eval_gt));
  env->setnocopy(env, "<", SVALUE.builtin_func(_eval_lt));
  env->setnocopy(env, ">=", SVALUE.builtin_func(_eval_ge));
  env->setnocopy(env, "<=", SVALUE.builtin_func(_eval_le));
  env->setnocopy(env, "not", SVALUE.builtin_func(_eval_not));
}

const struct _BuiltinsStatic BUILTIN = {
  .define_all = define_all,
};
