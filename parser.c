#include "parser.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "cons.h"
#include "svalue.h"
#include "tokenizer.h"
#include "util.h"

static bool is_bool(const char* token)
{
  return strcmp("#t", token) == 0 ||
         strcmp("#f", token) == 0;
}

static SValue* _parse_value(char *token, Tokenizer *t);
static SValue* _parse_cons(Tokenizer *t);

static SValue* _parse_cons(Tokenizer *t)
{
  SValue *res = NULL;
  char *token = t->next(t);
  if (NULL == token) {
    return SVALUE.errorf("Unexpected end of input");
  }

  SValue* car = _parse_value(token, t);
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

    car = _parse_value(token, t);
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
  } else {
    free(token);
  }
end:
  return res;
}

static SValue* _parse_value(char *token, Tokenizer *t)
{
	SValue *res = NULL;

  if (strcmp(token, "(") == 0) {
    res = _parse_cons(t);
    goto end;
  } else if (is_bool(token)) {
    bool val = token[1] == 't';
    res = SVALUE._bool(val);
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

static SValue* parse(const char *code)
{
	Tokenizer *t = TOKENIZER.from_string(code);
  SValue *result = NULL;

  while (t->has_next(t)) {
    char *token = t->next(t);
    SValue* sval = _parse_value(token, t);
    result = SVALUE.cons(sval, result);
  }

  result = CONS.list.reverse(result);

	t->release(&t);
	return result;

}

const Parser PARSE = parse;
