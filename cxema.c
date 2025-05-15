#include "cxema.h"

#include <stdlib.h>

#include "codex/fs/files.h"

#include "builtins.h"
#include "cons.h"
#include "env.h"
#include "evaluator.h"
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
  return CONS.list.take_last(self->interpret_all(self, code));
}

static SValue* interpret_all(Cxema *self, char *code)
{
	SValue *val = parse(self, code);
  CONS.list.eval_items(val, self->genv);
  return val;
}

static SValue* interpret_file(Cxema *self, char *filename)
{
  char *buf = FILES.read_to_str(filename);
  if (!buf)
    return SVALUE.errorf("failed to read file: %s", filename);
  SValue *res = self->interpret(self, buf);
  free(buf);
  return res;
}

static SValue* interpret_file_all(Cxema *self, char *filename)
{
  char *buf = FILES.read_to_str(filename);
  if (!buf)
    return SVALUE.errorf("failed to read file: %s", filename);
  SValue *res = self->interpret_all(self, buf);
  free(buf);
  return res;
}

static void release(Cxema **pself)
{
  Cxema *self = *pself;
  ENV.release(&self->genv);
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
    .interpret_all = interpret_all,
    .interpret_file = interpret_file,
    .interpret_file_all = interpret_file_all, 
		.release = release,
	},
	.form = form
};

