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

  return EVAL(env, SVALUE.copy(body));
}

static SValue* eval(Env *env, SValue *val)
{
  if (val->type == SVAL_TYPE_SYMBOL) {
    char *symbol = val->val.symbol;
    SValue* res = env->get(env, symbol);
    if (!res) {
      res = SVALUE.errorf("Undefined symbol \"%s\"", symbol);
    }
    SVALUE.release(&val);
    return res;
  }

  if (val->type == SVAL_TYPE_CONS) {
  start:
    SValue* res;
    SValue* car = val->val.cons.car;
    SValue* cdr = val->val.cons.cdr;
    switch (car->type) {
    case SVAL_TYPE_SPECIAL_FORM:
      res = SPECIAL_FORMS.apply(car, eval, env, cdr);
      free(val); // freeing cons envelope without contents
      SVALUE.release(&car);
      return res;
    case SVAL_TYPE_SYMBOL:
      val->val.cons.car = eval(env, car);
      goto start;
    case SVAL_TYPE_FUNC:
      SValue *arg = cdr;
      while (arg) {
        // TODO: check for cons?
        arg->val.cons.car = eval(env, arg->val.cons.car);
        arg = arg->val.cons.cdr;
      }

      if (car->val.func.is_builtin) {
        res = car->val.func.f.builtin(cdr);
      } else {
        res = _eval_scheme_func(car, cdr);
        
      }
      SVALUE.release(&val);
      return res;
    }
  }
  // not cons or symbol - just return as is
  return val;
}

static SValue* eval_all(Env *env, SValue *exprs)
{
  if (!CONS.is_list(exprs)) {
    return SVALUE.errorf("list expected. found \"%s\"", SVALUE.to_string(exprs));
  }

  SValue *head = exprs;
  SValue *res = NULL;
  while (head) {
    SValue *car = CONS.car(head);
    if (res)
      SVALUE.release(&res);
    res = EVAL(env, car);
    if (SVALUE.is_err(res))
      return res;
    head = CONS.cdr(head);
  }

  return res;
}

const Evaluator EVAL = eval;
const Evaluator EVAL_ALL = eval_all;
