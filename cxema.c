#include "cxema.h"

#include <stdlib.h>

#include "builtins.h"
#include "env.h"
#include "parser.h"
#include "svalue.h"
#include "special_form.h"
#include "util.h"

static SValue* eval(Cxema *self, SValue *val) {
  return EVAL(self->genv, val);
}

static SValue* parse(Cxema *self, char *code)
{
	return PARSE(code);
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

