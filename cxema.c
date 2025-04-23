#include "cxema.h"

#include <stdlib.h>
#include <errno.h>

#include "builtins.h"
#include "env.h"
#include "svalue.h"
#include "special_form.h"
#include "tokenizer.h"
#include "util.h"

static SValue* _parse_value(Cxema *self, char *token, Tokenizer *t);
static SValue* _parse_cons(Cxema *self, Tokenizer *t);

static SValue* _parse_cons(Cxema *self, Tokenizer *t)
{
  SValue *res = NULL;
  char *token = t->next(t);
  if (NULL == token) {
    return SVALUE.errorf("Unexpected end of input");
  }

  SValue* car = _parse_value(self, token, t);
  if (car->type == SVAL_TYPE_ERR) {
    return car;
  }
  res = SVALUE.cons(car, NULL);
  SValue* tail = res;
  while ((token = t->next(t)) != NULL && strcmp(token, ")") != 0) {
    if (NULL == token) {
      SVALUE.release(&res);
      return SVALUE.errorf("Unexpected end of input");
    }

    car = _parse_value(self, token, t);
    if (car->type == SVAL_TYPE_ERR) {
      SVALUE.release(&res);
      return car;
    }
    SValue* newtail = SVALUE.cons(car, NULL);
    tail->val.cons.cdr = newtail;
    tail = newtail;
  }
  if (NULL == token) {
    SVALUE.release(&res);
    return SVALUE.errorf("\")\" expected");
  }
end:
  return res;
}

static SValue* _parse_value(Cxema *self, char *token, Tokenizer *t)
{
	SValue *res = NULL;

  if (strcmp(token, "(") == 0) {
    res = _parse_cons(self, t);
    goto end;
  } else if (is_integer(token)) {
    int64_t num = strtol(token, NULL, 10);
    if (errno == ERANGE) {
      res = SVALUE.errorf("Value \"%s\" is too big", token);
      goto end;
    }
    if (errno == EINVAL) {
      // should be unreachable
      res = SVALUE.errorf("Invalid number \"%s\"", token);
      goto end;
    }
    res = SVALUE._int(num);
    goto end;
  } else if (is_float(token)) {
    char *end;
    double num = strtod(token, &end);
    if (errno == ERANGE) {
      res = SVALUE.errorf("Value \"%s\" is too big", token);
      goto end;
    }
    if (*end != '\0') {
      // should be unreachable
      res = SVALUE.errorf("Invalid number \"%s\"", token);
      goto end;
    }

    res = SVALUE._float(num);
    goto end;
  } else if (SPECIAL_FORMS.is_special_form(token)) {
    res = SPECIAL_FORMS.from_string(token);
    goto end;
  } else {
    // parse anything else as symbol
    // TODO: comments? quotes?
    res = SVALUE.symbol(token);
    goto end;
  }

  res = SVALUE.errorf("Undefined token: %s", token);

end:
  free(token);
  return res;
}

static SValue* eval(Cxema *self, SValue *val) {
  return EVAL(self->genv, val);
}

static SValue* parse(Cxema *self, char *code)
{
	Tokenizer *t = TOKENIZER.from_string(code);
  SValue *result = NULL;

  while (t->has_next(t)) {
    char *token = t->next(t);
    result = _parse_value(self, token, t);
    if (result->type == SVAL_TYPE_ERR)
      break;
  }

	t->release(&t);
	return result;

}

static SValue* interpret(Cxema *self, char *code)
{
	SValue *val = parse(self, code);
  return self->eval(self, val);
}

static void release(Cxema **pself)
{
  Cxema *self = *pself;
  self->genv->release(&self->genv);
	free(*pself);
	*pself = NULL;
}

static Cxema* form()
{
	Cxema *cxema = malloc(sizeof(*cxema));
	*cxema = CXEMA.prototype;

  cxema->genv = ENV.form();
  BUILTIN.define_all(cxema->genv);

	return cxema;
}

const struct _CxemaStatic CXEMA = {
	.prototype = {
		.parse = parse,
    .eval = eval,
		.interpret = interpret,
		.release = release,
	},
	.form = form
};

