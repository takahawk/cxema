#include "special_form.h"

#include <string.h>

#include "cons.h"
#include "evaluator.h"
#include "env.h"
#include "svalue.h"

static bool is_special_form(const char *token)
{
  return strcmp(token, "define") == 0 ||
         strcmp(token, "lambda") == 0;
}

static SValue* from_string(const char *symbol)
{
  if (strcmp(symbol, "define") == 0)
    return SVALUE.special_form(SPECIAL_FORM_DEFINE);
  if (strcmp(symbol, "lambda") == 0)
    return SVALUE.special_form(SPECIAL_FORM_LAMBDA);

  return SVALUE.errorf("unrecognized special form: \"%s\"", symbol);
}

static SValue* apply(SValue *sform, Evaluator eval, Env *env, SValue *args) {
  if (SPECIAL_FORM_DEFINE == sform->val.special_form)
    return SPECIAL_FORMS.define(eval, env, args);
  if (SPECIAL_FORM_LAMBDA == sform->val.special_form)
    return SPECIAL_FORMS.lambda(eval, env, args);

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
    // cons - define function
    // TODO: lambda?
    return SVALUE.errorf("not implemented");
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
  SValue *body = args->val.cons.cdr->val.cons.car;
  free(args);

  Env *func_env = ENV.form();
  func_env->parent = env;
  return SVALUE.scheme_func(func_env, params, body);
}

const struct _SpecialFormsStatic SPECIAL_FORMS = {
  .apply            = apply,
  .from_string      = from_string,
  .is_special_form  = is_special_form,
  .define           = define,
  .lambda           = lambda,
};
