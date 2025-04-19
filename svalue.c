#include "svalue.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

static SValue* errorf(const char *fmt, ...)
{
	SValue *sval = malloc(sizeof(*sval));
	sval->type = SVAL_TYPE_ERR;

	va_list va;
	va_start(va, fmt);
	char *error = malloc(1024);

	vsnprintf(error, 1023, fmt, va);

	error = realloc(error, strlen(error) + 1);

	va_end(va);

	sval->val.err = error;

	return sval;
}

static SValue* symbol(const char *symbol)
{
  SValue *sval = malloc(sizeof(*sval));
  sval->type = SVAL_TYPE_SYMBOL;
  sval->val.symbol = cpystr(symbol);

  return sval;
}

static SValue* cons(SValue *car, SValue *cdr)
{
  if (NULL == car) {
    return errorf("Empty cons are disallowed");
  }
  SValue *val = malloc(sizeof(*val));
  val->type = SVAL_TYPE_CONS;
  val->val.cons.car = car;
  val->val.cons.cdr = cdr;

  return val;
}

static SValue* _int(int64_t _int)
{
	SValue* value = malloc(sizeof(*value));

	value->type = SVAL_TYPE_INT;
	value->val._int = _int;

	return value;
}

static SValue* _float(double _float)
{
	SValue* value = malloc(sizeof(*value));

	value->type = SVAL_TYPE_FLOAT;
	value->val._float = _float;

	return value;
}

static SValue *func(SValue* (*eval) (Env*, SValue*, void*), void *ctx)
{
  SValue *value = malloc(sizeof(*value));

  value->type = SVAL_TYPE_FUNC;
  value->val.func = (SFunction) {
    .eval = eval,
    .ctx  = ctx
  };
}

static SValue *special_form(SpecialForm form)
{
  SValue *value = malloc(sizeof(*value));

  value->type = SVAL_TYPE_SPECIAL_FORM;
  value->val.special_form = form;

  return value;
}

static size_t _estimate_str_size(SValue *svalue)
{
  char buffer[256];
  if (NULL == svalue) {
    return 4; // nil
  }
  switch (svalue->type) {
  case SVAL_TYPE_SPECIAL_FORM:
    return strlen("<spec_form>") + 1;
  case SVAL_TYPE_SYMBOL:
    return strlen(svalue->val.symbol) + 1;
  case SVAL_TYPE_FUNC:
    return strlen("<function>") + 1;
  case SVAL_TYPE_INT:
    return sprintf(buffer, "%ld", svalue->val._int) + 1;
  case SVAL_TYPE_FLOAT:
    return sprintf(buffer, "%.10g", svalue->val._float) + 1;
  case SVAL_TYPE_ERR:
    return strlen(svalue->val.err) + 1;
  case SVAL_TYPE_CONS:
    return 1 + // (
           _estimate_str_size(svalue->val.cons.car) +
           3 + // dot and spaces
           _estimate_str_size(svalue->val.cons.cdr) +
           1 + // )
           1; // \0
  }
}

static int _sval_to_string(SValue *svalue, char *buffer)
{
  if (NULL == svalue) {
    return sprintf(buffer, "nil");
  }

  switch (svalue->type) {
  case SVAL_TYPE_SPECIAL_FORM:
    return sprintf(buffer, "<spec_form>");
  case SVAL_TYPE_SYMBOL:
    return sprintf(buffer, "%s", svalue->val.symbol);
  case SVAL_TYPE_FUNC:
    return sprintf(buffer, "<function>");
  case SVAL_TYPE_INT:
    return sprintf(buffer, "%ld", svalue->val._int);
  case SVAL_TYPE_FLOAT:
    return sprintf(buffer, "%.10g", svalue->val._float);
  case SVAL_TYPE_ERR:
    return sprintf(buffer, svalue->val.err);
  case SVAL_TYPE_CONS:
    int len = 0;
    len += sprintf(buffer, "(");
    len += _sval_to_string(svalue->val.cons.car, buffer + len);
    len += sprintf(buffer + len, " . ");
    len += _sval_to_string(svalue->val.cons.cdr, buffer + len);
    len += sprintf(buffer + len, ")");
    return len;
  }
}

static char* sval_to_string(SValue *svalue)
{
  size_t len = _estimate_str_size(svalue);
	char *buffer = malloc(len + 1);
  _sval_to_string(svalue, buffer);

	return buffer;
}

static void release(SValue **pself)
{
	SValue *self = *pself;

	switch (self->type) {
  case SVAL_TYPE_VOID:
    *pself = NULL;
    return;
	case SVAL_TYPE_ERR:
		free(self->val.err);
		break;
  case SVAL_TYPE_SYMBOL:
    free(self->val.symbol);
    break;
  case SVAL_TYPE_SPECIAL_FORM:
	case SVAL_TYPE_INT:
  case SVAL_TYPE_FLOAT:
		break;
  case SVAL_TYPE_CONS:
    SValue *car = self->val.cons.car;
    SValue *cdr = self->val.cons.cdr;
    if (NULL != car)
      SVALUE.release(&car);
    if (NULL != cdr)
      SVALUE.release(&cdr);
    break;
  case SVAL_TYPE_FUNC:
    void *ctx = self->val.func.ctx;
    if (ctx != NULL)
      free(ctx);
    break;
	}

	free(self);
	*pself = NULL;
}

const struct _SValueStatic SVALUE = {
  .symbol    = symbol,
  .func      = func,
	.errorf    = errorf,
	._int      = _int,
  ._float    = _float,
  .cons      = cons,

	.to_string = sval_to_string,

	.release   = release
};

static char* sval_type_to_string(SValueType type)
{
	switch (type) {
  case SVAL_TYPE_VOID:
    return "Void";
	case SVAL_TYPE_INT:
		return "Integer";
  case SVAL_TYPE_FLOAT:
    return "Float";
	case SVAL_TYPE_ERR:
		return "Error";
  case SVAL_TYPE_CONS:
    return "Cons";
  case SVAL_TYPE_SYMBOL:
    return "Symbol";
  case SVAL_TYPE_FUNC:
    return "Function";
  case SVAL_TYPE_SPECIAL_FORM:
    return "Special Form";
	default:
		return "Unknown";
	}
}

const SValue SVAL_VOID = {
  .type = SVAL_TYPE_VOID,
};

const struct _SValueTypeStatic SVALUE_TYPE = {
	.to_string = sval_type_to_string
};

static SValue* apply(SFunction func, Env *env, SValue *svalue)
{
  return func.eval(env, svalue, func.ctx);
}

const struct _SFunctionStatic SFUNCTION = {
  .apply = apply,
};
