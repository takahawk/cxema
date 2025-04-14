#include "svalue.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static SValue* errorf(const char *fmt, ...) {
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

static SValue* cons(SValue *car, SValue *cdr) {
  if (NULL == car) {
    return errorf("Empty cons are disallowed");
  }
  SValue *val = malloc(sizeof(*val));
  val->type = SVAL_TYPE_CONS;
  val->val.cons.car = car;
  val->val.cons.cdr = cdr;

  return val;
}

static SValue* num(long num) {
	SValue* value = malloc(sizeof(*value));

	value->type = SVAL_TYPE_NUM;
	value->val.num = num;

	return value;
}


static size_t _estimate_str_size(SValue *svalue) {
  if (NULL == svalue) {
    return 3; // nil
  }
  switch (svalue->type) {
  case SVAL_TYPE_NUM:
    return floor(log10(svalue->val.num));
  case SVAL_TYPE_ERR:
    return strlen(svalue->val.err);
  case SVAL_TYPE_CONS:
    return 1 + // (
           _estimate_str_size(svalue->val.cons.car) +
           3 + // dot and spaces
           _estimate_str_size(svalue->val.cons.cdr) +
           1; // )
  }
}

static int _sval_to_string(SValue *svalue, char *buffer) {
  if (NULL == svalue) {
    return sprintf(buffer, "nil");
  }

  switch (svalue->type) {
  case SVAL_TYPE_NUM:
    return sprintf(buffer, "%ld", svalue->val.num);
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

static char* sval_to_string(SValue *svalue) {
  size_t len = _estimate_str_size(svalue);
	char *buffer = malloc(len + 1);
  _sval_to_string(svalue, buffer);

	return buffer;
}

static void release(SValue **pself) {
	SValue *self = *pself;

	switch (self->type) {
	case SVAL_TYPE_ERR:
		free(self->val.err);
		break;
	case SVAL_TYPE_NUM:
		break;
  case SVAL_TYPE_CONS:
    SValue *car = self->val.cons.car;
    SValue *cdr = self->val.cons.cdr;
    if (NULL != car)
      SVALUE.release(&car);
    if (NULL != cdr)
      SVALUE.release(&cdr);
    break;
	}

	free(self);
	*pself = NULL;
}

const struct _SValueStatic SVALUE = {
	.to_string = sval_to_string,
	.errorf    = errorf,
	.num       = num,
  .cons      = cons,
	.release   = release
};

static char* sval_type_to_string(SValueType type) {
	switch (type) {
	case SVAL_TYPE_NUM:
		return "Integer";
	case SVAL_TYPE_ERR:
		return "Error";
  case SVAL_TYPE_CONS:
    return "Cons";
	default:
		return "Unknown";
	}
}

const struct _SValueTypeStatic SVALUE_TYPE = {
	.to_string = sval_type_to_string
};
