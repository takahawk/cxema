#include "env.h"

#include "codex/allocators/allocator.h"
#include "codex/ds/array.h"

#include "svalue.h"
#include "util.h"

static void set(Env *self, char *symbol, SValue *val)
{
  Array*/*char**/ ss = self->symbols;
  Array*/*SValue**/ vals = self->values;
  size_t i;
  for (i = 0; i < ss->len; ++i) {
    if (strcmp(symbol, ss->get(ss, i)) == 0) {
      break;
    }
  }

  if (i == ss->len) {
    ss->add(ss, cpystr(symbol));
    vals->add(vals, NULL);
  }

  vals->set(vals, i, &val);
}

static SValue* get(Env *self, char *symbol)
{
  Array*/*char**/ ss = self->symbols;
  for (int i = 0; i < ss->len; ++i) {
    if (strcmp(symbol, ss->get(ss, i)) == 0) {
      SValue **pval = self->values->get(self->values, i);
      return *pval;
    }
  }

  return NULL;
}

static Env* form()
{
  Env* env = malloc(sizeof(Env));

  *env = ENV.prototype;
  env->symbols = ARRAY.form(&STD_ALLOCATOR, sizeof(char*));
  env->symbols->item_release = JUST_FREE_IT;
  env->values = ARRAY.form(&STD_ALLOCATOR, sizeof(SValue*));
  env->values->item_release = JUST_FREE_IT;

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
    .set     = set,
    .get     = get,
    .release = release,
  },

  .form = form
};
