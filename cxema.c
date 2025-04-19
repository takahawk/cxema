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
  } else if (SPECIAL_FORMS.is_special_token(token)) {


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

static SValue* eval(Cxema *self, Env *env, SValue *val)
{
  switch (val->type) {
  case SVAL_TYPE_CONS:
    SValue* res;
    SValue* car = val->val.cons.car;
    SValue* cdr = val->val.cons.cdr;
    // TODO: lambdas?
    if (car->type != SVAL_TYPE_SYMBOL) {
      res = SVALUE.errorf("Symbol expected, found \"%s\"",
                                  SVALUE.to_string(car));
      goto end;
    }

    char *symbol = car->val.symbol;
    SValue* defined = env->get(env, symbol);

    if (!defined) {
      res = SVALUE.errorf("Undefined symbol \"%s\"", symbol);
      goto end;
    }

    if (defined->type != SVAL_TYPE_FUNC) {
      res = SVALUE.errorf("%s=%s is not a function", symbol,
                          SVALUE.to_string(defined));
      goto end;
    }

    SValue *arg = cdr;
    while (arg) {
      // TODO: check for cons?
      arg->val.cons.car = self->eval(self, self->genv, arg->val.cons.car);
      arg = arg->val.cons.cdr;
    }

    res = SFUNCTION.apply(defined->val.func, env, cdr);
  end:
    SVALUE.release(&val);
    return res;
  default:
    return val;
  }
}

static SValue* interpret(Cxema *self, char *code)
{
	SValue *val = parse(self, code);
  return self->eval(self, self->genv, val);
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

