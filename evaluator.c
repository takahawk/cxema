#include "evaluator.h"

#include "cons.h"
#include "env.h"
#include "svalue.h"

static SValue* _eval_scheme_func(SValue *func, SValue *args)
{
reframe:
  Env *env = func->val.func.f.scheme.env;
  SValue *params = func->val.func.f.scheme.params;
  SValue *body = func->val.func.f.scheme.body;

  size_t expected_len = CONS.list.len(params);
  size_t actual_len   = CONS.list.len(args);

  if (expected_len != actual_len) {
    SVALUE.release(&args);
    SVALUE.release(&func);
    return SVALUE.errorf("Wrong number of arguments (expected=%d got=%d)",
                         expected_len,
                         actual_len);
  }

  SValue *arg = args;
  SValue *param = params;
  for (;
       param && arg;
       param = CONS.cdr(param), arg = CONS.cdr(arg)) {
    env->setnocopy(env, CONS.car(param)->val.symbol, CONS.car(arg));
  }

  CONS.list.release_envelope(&args);
  SVALUE.release(&params);
  SValue *last = EVAL_ALL_BUT_ONE(env, body);

  // tail call optimization in its grace
tco:
  if (SVALUE.is_cons(last)) {
    SValue *car = CONS.car(last);
    SValue *cdr = CONS.cdr(last);

    while (SVALUE.is_special_form(car)) {
      free(last);
      last = SPECIAL_FORMS.apply(car, env, cdr);
      goto tco;
    }

    if (SVALUE.is_symbol(car))
      car = last->val.cons.car = EVAL(env, car);

    if (SVALUE.is_scheme_func(car)) {
      CONS.list.eval_items(cdr, env);

      free(last);
      free(func);
      env->release(&env);

      func = car;
      args = cdr;
      goto reframe;
    }
  }

  SValue *res = EVAL(env, last);
  env->release(&env);
  free(func);

  return res;
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
    SValue* res;
    SValue* car = val->val.cons.car;
    SValue* cdr = val->val.cons.cdr;
    // free cons envelope
    free(val);
  start:
    switch (car->type) {
    case SVAL_TYPE_SPECIAL_FORM:
      return EVAL(env, SPECIAL_FORMS.apply(car, env, cdr));
    case SVAL_TYPE_CONS:
    case SVAL_TYPE_SYMBOL:
      car = eval(env, car);
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
        SVALUE.release(&cdr);
      } else {
        res = _eval_scheme_func(car, cdr);
      }
      return res;
    }
  }
  // not cons or symbol - just return as is
  return val;
}

static SValue* eval_all_but_one(Env *env, SValue *exprs)
{
  if (!CONS.is_list(exprs) || !exprs) {
    return SVALUE.errorf("non-empty list expected. found \"%s\"", SVALUE.to_string(exprs));
  }

  SValue *head = exprs;
  while (CONS.cdr(head)) {
    // all
    SValue *car = CONS.car(head);
    SValue *cdr = CONS.cdr(head);
    free(head);
    SValue *res = EVAL(env, car);
    if (SVALUE.is_err(res)) {
      SVALUE.release(&head);
      return res;
    }
    head = cdr;
  }

  return head;
}

static SValue* eval_all(Env *env, SValue *exprs)
{
  return EVAL(env, eval_all_but_one(env, exprs));
}

const Evaluator EVAL = eval;
const Evaluator EVAL_ALL = eval_all;
const Evaluator EVAL_ALL_BUT_ONE = eval_all_but_one;
