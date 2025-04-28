#include "special_form.h"

#include <string.h>

#include "cons.h"
#include "evaluator.h"
#include "env.h"
#include "svalue.h"

static bool is_special_form(const char *token)
{
  return strcmp(token, "define") == 0 ||
         strcmp(token, "lambda") == 0 ||
         strcmp(token, "cond")   == 0;
}

static SValue* from_string(const char *symbol)
{
  if (strcmp(symbol, "define") == 0)
    return SVALUE.special_form(SPECIAL_FORM_DEFINE);
  if (strcmp(symbol, "lambda") == 0)
    return SVALUE.special_form(SPECIAL_FORM_LAMBDA);
  if (strcmp(symbol, "cond") == 0)
    return SVALUE.special_form(SPECIAL_FORM_COND);

  return SVALUE.errorf("unrecognized special form: \"%s\"", symbol);
}

static SValue* apply(SValue *sform, Evaluator eval, Env *env, SValue *args) {
  switch (sform->val.special_form) {
  case SPECIAL_FORM_DEFINE: 
    return SPECIAL_FORMS.define(eval, env, args);
  case SPECIAL_FORM_LAMBDA:
    return SPECIAL_FORMS.lambda(eval, env, args);
  case SPECIAL_FORM_COND:
    return SPECIAL_FORMS.cond(eval, env, args);
  }

  return SVALUE.errorf("unrecognized special form");
}

static SValue* define(Evaluator eval, Env *env, SValue *args)
{
  if (!args || !args->val.cons.cdr || args->val.cons.cdr->val.cons.cdr) {
    return SVALUE.errorf("exactly two arguments are expected (define)");
  }

  SValue *head = args->val.cons.car;
  SValue *body = args->val.cons.cdr->val.cons.car;

  if (SVAL_TYPE_SYMBOL == head->type) {
    // just a symbol - evaluate args beforehand and just store val
    SValue *sval = eval(env, body);
    if (SVAL_TYPE_ERR == sval->type) {
      return sval;
    }
    env->set(env,
             head->val.symbol,
             sval);
    return &SVAL_VOID;
  } else if (SVAL_TYPE_CONS == head->type) {
    SValue *name = head->val.cons.car;
    if (SVAL_TYPE_SYMBOL != name->type) {
      return SVALUE.errorf("expected symbol, got: %s (type=%s)",
                           SVALUE.to_string(name),
                           SVALUE_TYPE.to_string(name->type));
    }
    args->val.cons.car = args->val.cons.car->val.cons.cdr;
    SValue *func = SPECIAL_FORMS.lambda(eval, env, args);
    if (SVAL_TYPE_ERR == func->type) {
      SVALUE.release(&args);
      return func;
    }
    env->set(env, name->val.symbol, func);
    return &SVAL_VOID;
  } else {
    return SVALUE.errorf("expected list or symbol");
  }
}

static SValue* lambda(Evaluator eval, Env *env, SValue *args)
{
  if (!CONS.is_list(args) || CONS.list.len(args) != 2) {
    return SVALUE.errorf("expected list with exactly 2 arguments (lambda) (got %d)",
                         CONS.list.len(args));

  }

  SValue *params = args->val.cons.car;

  if (!CONS.is_list(params)) {
    return SVALUE.errorf("expected list. got: %s (type=%s)", 
                         SVALUE.to_string(params),
                         SVALUE_TYPE.to_string(params->type));
  }

  if (!CONS.list.is_all(params, SVALUE.is_symbol)) {
    return SVALUE.errorf("expected all parameters to be symbols (lambda)");
  }
  SValue *body = CONS.cdar(args);
  free(args);

  Env *func_env = ENV.form();
  func_env->parent = env;
  return SVALUE.scheme_func(func_env, params, body);
}

static SValue* cond(Evaluator eval, Env *env, SValue *args)
{
  if (!args) {
    return SVALUE.errorf("at least one condition/expression pair is expected (cond)");
  }

  SValue *head = args;
  SValue *res = &SVAL_VOID;
  while (head) {
    SValue *pair = CONS.car(head);
    if (!SVALUE.is_cons(pair)) {
      res = SVALUE.typeerr(pair, SVAL_TYPE_CONS);
      goto end;
    }

    SValue *cond = CONS.car(pair);


    SValue *cond_res = EVAL(env, cond);
    if (cond_res->type == SVAL_TYPE_ERR) {
      res = cond_res;
      goto end;
    }

    if (!SVALUE.is_false(cond_res)) {
      // condition is true - evaluate appropriate expression
      SValue *expr = CONS.cdr(pair);
      res = EVAL_ALL(env, expr);
      goto end;
    }
    head = CONS.cdr(head);
  }
end:
  // SVALUE.release(&args);
  return res;
}

const struct _SpecialFormsStatic SPECIAL_FORMS = {
  .apply            = apply,
  .from_string      = from_string,
  .is_special_form  = is_special_form,

  .define           = define,
  .lambda           = lambda,
  .cond             = cond,
};
