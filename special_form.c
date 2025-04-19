#include "special_form.h"

#include <string.h>

#include "cxema.h"
#include "env.h"
#include "svalue.h"

static bool is_special_token(const char *token)
{
  return strcmp(token, "define") == 0;
}

static SValue* from_string(const char *symbol)
{
  if (strcmp(symbol, "define") == 0)
    return SVALUE.special_form(SPECIAL_FORM_DEFINE);

  return SVALUE.errorf("unrecognized special form: \"%s\"", symbol);
}

static SValue* apply(SValue *sform, Cxema *cx, SValue *args) {
  if (SPECIAL_FORM_DEFINE == sform->val.special_form)
    return SPECIAL_FORMS.define(cx, args);

  return SVALUE.errorf("unrecognized special form");
}

static SValue* define(Cxema *cx, SValue *args)
{
  if (!args || !args->val.cons.cdr || args->val.cons.cdr->val.cons.cdr) {
    return SVALUE.errorf("exactly two arguments are expected (define)");
  }

  SValue *head = args->val.cons.car;
  SValue *body = args->val.cons.cdr->val.cons.car;

  if (SVAL_TYPE_SYMBOL == head->type) {
    // just a symbol - evaluate args beforehand and just store val
    SValue *sval = cx->eval(cx, cx->genv, body);
    if (SVAL_TYPE_ERR == sval->type) {
      return sval;
    }
    cx->genv->set(cx->genv,
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

const struct _SpecialFormsStatic SPECIAL_FORMS = {
  .apply            = apply,
  .from_string      = from_string,
  .is_special_token = is_special_token,
  .define           = define,
};
