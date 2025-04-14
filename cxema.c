#include "cxema.h"

#include <stdlib.h>
#include <errno.h>

#include "tokenizer.h"
#include "util.h"

static SValue* _parse_value(Cxema *self, char *token, Tokenizer *t);
static SValue* _parse_cons(Cxema *self, Tokenizer *t);

static SValue* _parse_cons(Cxema *self, Tokenizer *t) {
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

static SValue* _parse_value(Cxema *self, char *token, Tokenizer *t) {
	SValue *res = NULL;

  if (strcmp(token, "(") == 0) {
    res = _parse_cons(self, t);
    goto end;
  } else if (is_integer(token)) {
    long num = strtol(token, NULL, 10);
    if (errno == ERANGE) {
      res = SVALUE.errorf("Value \"%s\" is too big", token);
      goto end;
    } else if (errno == EINVAL) {
      res = SVALUE.errorf("Invalid number \"%s\"", token);
      goto end;
    } else {
      res = SVALUE.num(num);
      goto end;
    }
  }

  res = SVALUE.errorf("Undefined token: %s", token);

end:
  free(token);
  return res;
}

static SValue* parse(Cxema *self, char *code) {
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

static SValue* interpret(Cxema *self, char *code) {
	return parse(self, code);

}

static void release(Cxema **pself) {
	free(*pself);
	*pself = NULL;
}

static Cxema* form() {
	Cxema *cxema = malloc(sizeof(*cxema));
	*cxema = CXEMA.prototype;
	return cxema;
}

const struct _CxemaStatic CXEMA = {
	.prototype = {
		.parse = parse,
		.interpret = interpret,
		.release = release,
	},
	.form = form
};

