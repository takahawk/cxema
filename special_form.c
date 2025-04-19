#include "special_form.h"

#include "cxema.h"
#include "env.h"
#include "svalue.h"

SValue* define(Cxema *cx, SValue *args)
{
  if (!args || !args->val.cons.cdr || args->val.cons.cdr->val.cons.cdr) {
    return SVALUE.errorf("exactly two arguments are expected (define)");
  }

  SValue *head = args->val.cons.car;
  SValue *body = args->val.cons.cdr->val.cons.car;

  if (SVAL_TYPE_SYMBOL == head->type) {
    // just a symbol - evaluate args beforehand and just store val
    SValue *sval = cx->eval(cx, body);
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

extern const struct _SpecialFormsStatic SPECIAL_FORMS;
