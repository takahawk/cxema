#include "evaluator.h"

#include "cons.h"
#include "env.h"
#include "svalue.h"

static SValue* _eval_scheme_func(SValue *func, SValue *args)
{
  Env *env = func->val.func.f.scheme.env;
  SValue *params = func->val.func.f.scheme.params;
  SValue *body = func->val.func.f.scheme.body;

  size_t expected_len = CONS.list.len(params);
  size_t actual_len   = CONS.list.len(args);

  if (expected_len != actual_len) {
    return SVALUE.errorf("Wrong number of arguments (expected=%d got=%d)",
                         expected_len,
                         actual_len);
  }

  for (;
       params && args;
       params = params->val.cons.cdr, args = args->val.cons.cdr) {
    env->set(env, params->val.cons.car->val.symbol, args->val.cons.car);
  }

  return EVAL(env, body);
}

static SValue* eval(Env *env, SValue *val)
{
  switch (val->type) {
  case SVAL_TYPE_CONS:
    SValue* res;
    SValue* car = val->val.cons.car;
    SValue* cdr = val->val.cons.cdr;
    // TODO: lambdas?
    
    if (car->type == SVAL_TYPE_SPECIAL_FORM) {
        res = SPECIAL_FORMS.apply(car, eval, env, cdr);
        free(val); // freeing cons envelope without contents
        SVALUE.release(&car);
        return res;
    }
    if (car->type != SVAL_TYPE_SYMBOL) {
      res = SVALUE.errorf("Symbol expected, found \"%s\"",
                          SVALUE.to_string(car));
      goto end;
    }

    char *symbol = car->val.symbol;
    SValue* defined = env->get(env, symbol);

    if (!defined) {
      res = SVALUE.errorf("Undefined symbol \"%s\"", symbol);
      goto end;
    }

    if (defined->type != SVAL_TYPE_FUNC) {
      res = SVALUE.errorf("%s=%s is not a function", symbol,
                          SVALUE.to_string(defined));
      goto end;
    }

    SValue *arg = cdr;
    while (arg) {
      // TODO: check for cons?
      arg->val.cons.car = eval(env, arg->val.cons.car);
      arg = arg->val.cons.cdr;
    }

    if (defined->val.func.is_builtin) {
      res = defined->val.func.f.builtin(cdr);
    } else {
      res = _eval_scheme_func(defined, cdr);
    }
  end:
    SVALUE.release(&val);
    return res;
  case SVAL_TYPE_SYMBOL:
    res = env->get(env, val->val.symbol);
    SVALUE.release(&val);
    return res;
  default:
    return val;
  }
}

const Evaluator EVAL = eval;
