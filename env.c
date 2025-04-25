#include "env.h"

#include "codex/mem/allocator.h"
#include "codex/mem/release_cb.h"
#include "codex/ds/array.h"

#include "svalue.h"
#include "util.h"

static void _release_sval(Allocator *a, void **sval)
{
  SValue *val = *(SValue **) sval;
  SVALUE.release(&val);
  *sval = NULL;
}

static void setnocopy(Env *self, char *symbol, SValue *val)
{

  Array*/*char**/ ss = self->symbols;
  Array*/*SValue**/ vals = self->values;
  size_t i;
  for (i = 0; i < ss->len; ++i) {
    if (strcmp(symbol, *(char **) ss->get(ss, i)) == 0) {
      break;
    }
  }

  if (i == ss->len) {
    symbol = cpystr(symbol);
    ss->add(ss, &symbol);
    vals->add(vals, NULL);
  } else {
    SValue *prev = *(SValue **) vals->get(vals, i);
    SVALUE.release(&prev);
  }

  vals->set(vals, i, &val);
}

static void set(Env *self, char *symbol, SValue *val)
{
  val = SVALUE.copy(val);
  setnocopy(self, symbol, val);
}

static SValue* get(Env *self, char *symbol)
{
  Array*/*char**/ ss = self->symbols;
  for (int i = 0; i < ss->len; ++i) {
    if (strcmp(symbol, *(char **) ss->get(ss, i)) == 0) {
      SValue *val = *(SValue **) self->values->get(self->values, i);
      return SVALUE.copy(val);
    }
  }

  if (self->parent)
    return self->parent->get(self->parent, symbol);

  return SVALUE.errorf("Unknown symbol: \"%s\"", symbol);
}

static Env* form()
{
  Env* env = malloc(sizeof(Env));

  *env = ENV.prototype;
  env->symbols = ARRAY.form(&STD_ALLOCATOR, sizeof(char*));
  env->symbols->release_cb = RELEASE_CB.form_free_cb(&STD_ALLOCATOR);
  env->values = ARRAY.form(&STD_ALLOCATOR, sizeof(SValue*));
  // TODO: release svalues
  env->values->release_cb = RELEASE_CB.form(&STD_ALLOCATOR, _release_sval);

  return env;
}

static Env* copy(Env *original)
{

  Env* env = form();

  Array *symbols = original->symbols;
  Array *values  = original->values;

  for (int i = 0; i < original->symbols->len; ++i) {
    char* symbol = *(char **) symbols->get(symbols, i);
    SValue* value = *(SValue **) values->get(values, i);

    set(env, symbol, value);
  }
  env->parent = original->parent;

  return env;
}

static void release(Env **pself)
{
  Env *self = *pself;

  self->symbols->release(&self->symbols);
  self->values->release(&self->values);
  free(self);
  *pself = self;
}

const struct _EnvStatic ENV = {
  .prototype = {
    .parent  = NULL,

    .set       = set,
    .setnocopy = setnocopy,
    .get       = get,
    .release   = release,
  },

  .form = form,
  .copy = copy,
};
