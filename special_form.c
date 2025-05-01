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
         strcmp(token, "cond")   == 0 ||
         strcmp(token, "if")     == 0 ||
         strcmp(token, "and")    == 0 ||
         strcmp(token, "or")     == 0;
}

static SValue* from_string(const char *symbol)
{
  if (strcmp(symbol, "define") == 0)
    return SVALUE.special_form(SPECIAL_FORM_DEFINE);
  if (strcmp(symbol, "lambda") == 0)
    return SVALUE.special_form(SPECIAL_FORM_LAMBDA);
  if (strcmp(symbol, "cond") == 0)
    return SVALUE.special_form(SPECIAL_FORM_COND);
  if (strcmp(symbol, "if") == 0)
    return SVALUE.special_form(SPECIAL_FORM_IF);
  if (strcmp(symbol, "and") == 0)
    return SVALUE.special_form(SPECIAL_FORM_AND);
  if (strcmp(symbol, "or") == 0)
    return SVALUE.special_form(SPECIAL_FORM_OR);

  return SVALUE.errorf("unrecognized special form: \"%s\"", symbol);
}

static SValue* apply(SValue *sform, Env *env, SValue *args) {
  SpecialForm form = sform->val.special_form;
  SVALUE.release(&sform);
  switch (form) {
  case SPECIAL_FORM_DEFINE: 
    return SPECIAL_FORMS.define(env, args);
  case SPECIAL_FORM_LAMBDA:
    return SPECIAL_FORMS.lambda(env, args);
  case SPECIAL_FORM_COND:
    return SPECIAL_FORMS.cond(env, args);
  case SPECIAL_FORM_IF:
    return SPECIAL_FORMS._if(env, args);
  case SPECIAL_FORM_AND:
    return SPECIAL_FORMS.and(env, args);
  case SPECIAL_FORM_OR:
    return SPECIAL_FORMS.or(env, args);
  }

  return SVALUE.errorf("unrecognized special form");
}

static SValue* define(Env *env, SValue *args)
{
  if (!args || !args->val.cons.cdr || args->val.cons.cdr->val.cons.cdr) {
    return SVALUE.errorf("exactly two arguments are expected (define)");
  }

  SValue *head = CONS.car(args);
  SValue *body = CONS.cdar(args);

  if (SVAL_TYPE_SYMBOL == head->type) {
    // just a symbol - evaluate args beforehand and just store val
    SValue *sval = EVAL(env, body);
    if (SVAL_TYPE_ERR == sval->type) {
      return sval;
    }
    env->setnocopy(env, head->val.symbol, sval);
    SVALUE.release(&head);
    CONS.list.release_envelope(&args);
    return &SVAL_VOID;
  } else if (SVAL_TYPE_CONS == head->type) {
    SValue *name = head->val.cons.car;
    if (SVAL_TYPE_SYMBOL != name->type) {
      SValue* res = SVALUE.errorf("expected symbol, got: %s (type=%s)",
                                  SVALUE.to_string(name),
                                  SVALUE_TYPE.to_string(name->type));
      SVALUE.release(&args);
      return res;
    }
    args->val.cons.car = args->val.cons.car->val.cons.cdr;
    SValue *func = SPECIAL_FORMS.lambda(env, args);
    if (SVAL_TYPE_ERR == func->type) {
      return func;
    }
    env->setnocopy(env, name->val.symbol, func);
    return &SVAL_VOID;
  } else {
    return SVALUE.errorf("expected list or symbol");
  }
}

static SValue* lambda(Env *env, SValue *args)
{
  if (!CONS.is_list(args) || CONS.list.len(args) != 2) {
    return SVALUE.errorf("expected list with exactly 2 arguments (lambda) (got %d)",
                         CONS.list.len(args));

  }

  SValue *params = CONS.car(args);
  SValue *body = CONS.cdar(args);
  CONS.list.release_envelope(&args);

  if (!CONS.is_list(params)) {
    return SVALUE.errorf("expected list. got: %s (type=%s)", 
                         SVALUE.to_string(params),
                         SVALUE_TYPE.to_string(params->type));
  }

  if (!CONS.list.is_all(params, SVALUE.is_symbol)) {
    return SVALUE.errorf("expected all parameters to be symbols (lambda)");
  }


  Env *func_env = ENV.form();
  func_env->parent = env;
  return SVALUE.scheme_func(func_env, params, body);
}

static SValue* cond(Env *env, SValue *args)
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
    if (SVALUE.is_symbol(cond) && 
        strcmp(cond->val.symbol, "else") == 0) {
      if (CONS.cdr(head) != NULL) {
        res = SVALUE.errorf("misplaced else clause - should be the last one in cond");
        goto end;
      }
    } else {
      SValue *cond_res = EVAL(env, cond);
      if (cond_res->type == SVAL_TYPE_ERR) {
        res = cond_res;
        goto end;
      }
      if (SVALUE.is_false(cond_res)) {
        head = CONS.cdr(head);
        continue;
      }
    }

    // condition is true - evaluate appropriate expression
    SValue *expr = CONS.cdr(pair);
    res = EVAL_ALL(env, expr);
    goto end;

    head = CONS.cdr(head);
  }
end:
  return res;
}

static SValue* _if(Env *env, SValue *args)
{
  int arglen = CONS.list.len(args);
  if (arglen < 2) {
    return SVALUE.errorf("if expected at least 2 arguments (if <condition> <then> [<else>]) (got %d)",
                         CONS.list.len(args));
  }

  SValue *cond = CONS.car(args);
  SValue *cond_res = EVAL(env, cond);

  if (cond_res->type == SVAL_TYPE_ERR) {
    return cond_res;
  }

  if (SVALUE.is_false(cond_res)) {
    if (arglen == 2) // no else clause
      return &SVAL_VOID;

    SValue *_else = CONS.cddar(args);

    return EVAL(env, _else);
  } else {
    SValue *then = CONS.cdar(args);
    return EVAL(env, then);
  }
}

static SValue* and(Env *env, SValue *args)
{
  if (!args)
    return SVALUE._bool(true);

  SValue *res = NULL;
  while (args) {
    if (res)
      SVALUE.release(&res);
    res = EVAL(env, CONS.car(args));
    if (SVALUE.is_false(res))
      return res;

    args = CONS.cdr(args);
  }

  return res;
}

static SValue* or(Env *env, SValue *args)
{
  if (!args)
    return SVALUE._bool(false);

  SValue *res = NULL;
  while (args) {
    if (res)
      SVALUE.release(&res);
    res = EVAL(env, CONS.car(args));
    if (!SVALUE.is_false(res))
      return res;

    args = CONS.cdr(args);
  }

  return res;
}

const struct _SpecialFormsStatic SPECIAL_FORMS = {
  .apply            = apply,
  .from_string      = from_string,
  .is_special_form  = is_special_form,

  .define           = define,
  .lambda           = lambda,
  .cond             = cond,
  ._if              = _if,
  .and              = and,
  .or               = or,
};
