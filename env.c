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

static void setnocopy(Rc* /*Env**/ rcself, char *symbol, SValue *val)
{
  Env *self = rcself->data;
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

static void set(Rc* /*Env**/ rcself, char *symbol, SValue *val)
{
  val = SVALUE.copy(val);
  setnocopy(rcself, symbol, val);
}

static SValue* get(Rc* /*Env**/ rcself, char *symbol)
{
  Env *self = rcself->data;
  Array*/*char**/ ss = self->symbols;
  for (int i = 0; i < ss->len; ++i) {
    if (strcmp(symbol, *(char **) ss->get(ss, i)) == 0) {
      SValue *val = *(SValue **) self->values->get(self->values, i);
      return SVALUE.copy(val);
    }
  }

  if (self->rcparent) {
    return ENV.get(self->rcparent, symbol);
  }
  return SVALUE.errorf("Unknown symbol: \"%s\"", symbol);
}

static void _release_env(Allocator *a, void **env)
{
  Env *self = *env;

  self->symbols->release(&self->symbols);
  self->values->release(&self->values);
  if (self->rcparent) {
    ENV.release(&self->rcparent);
  }
  free(self);
  *env = NULL;
}

static Rc* /*Env**/ form()
{
  Env* env = malloc(sizeof(Env));

  *env = ENV.prototype;
  env->symbols = ARRAY.form(sizeof(char*));
  env->symbols->release_cb = RELEASE_CB.form_free_cb(&STD_ALLOCATOR);
  env->values = ARRAY.form(sizeof(SValue*));
  env->values->release_cb = RELEASE_CB.form(&STD_ALLOCATOR, _release_sval);

  Rc* rcenv = RC.form(env, RELEASE_CB.form(&STD_ALLOCATOR, _release_env));

  return rcenv;
}

static Rc* /*Env**/ form_child(Rc* /*Env**/ rc_parent) 
{
  Rc *rc_env = ENV.form();
  Env *env = rc_env->data;
  env->rcparent = (rc_parent && ENV.get_parent(rc_parent)) 
    ? RC.retain(rc_parent)
    : rc_parent; // weak reference for top-level environment

  return rc_env;
}

static Rc* /*Env**/ get_parent(Rc/*Env**/ *rc_env)
{
  Env* env = rc_env->data;
  return env->rcparent;
}

static void release(Rc*/*Env**/ *pself)
{
  RC.release(pself);
}

const struct _EnvStatic ENV = {
  .prototype = {
    .rcparent  = NULL,
  },

  .form       = form,
  .form_child = form_child,

  .get_parent = get_parent,

  .set       = set,
  .setnocopy = setnocopy,
  .get       = get,
  .release   = release,
};
