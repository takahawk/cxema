#include "cons.h"

#include "svalue.h"

#include <stdlib.h>

static SValue *car(SValue *val)
{
  return val->val.cons.car;
}

static SValue *cdr(SValue *val)
{
  return val->val.cons.cdr;
}

static SValue *cdar(SValue *val)
{
  return val->val.cons.cdr->val.cons.car;
}

static SValue *cddar(SValue *val)
{
  return val->val.cons.cdr->val.cons.cdr->val.cons.car;
}

static bool is_list(SValue *val) 
{
  return val == NULL || 
         (val->type == SVAL_TYPE_CONS && 
         is_list(val->val.cons.cdr));
}

static void release_envelope(SValue **pval)
{
  SValue *val = *pval;
  if (!val)
    return;

  SValue *cdr = CONS.cdr(val);
  release_envelope(&cdr);
  free(val);
  *pval = NULL;
}

static size_t list_len(SValue *val) 
{
  // TODO: iterate?
  if (!val)
    return 0;
  return 1 + list_len(val->val.cons.cdr);
}

static bool list_is_all(SValue *sval, bool (*cb) (SValue *val)) 
{
  if (!sval)
    return true;
  // TODO: iterate?
  return cb(sval->val.cons.car) && list_is_all(sval->val.cons.cdr, cb);
}

const struct _ConsStatic CONS = {
  .car = car,
  .cdr = cdr,
  .cdar = cdar,
  .cddar = cddar,

  .is_list = is_list,
  .list = {
    .release_envelope = release_envelope,
    .len = list_len,
    .is_all = list_is_all,
  }
};
