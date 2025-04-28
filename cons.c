#include "cons.h"

#include "svalue.h"
#include <stdio.h>

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

static bool is_list(SValue *val) 
{
  return val == NULL || 
         (val->type == SVAL_TYPE_CONS && 
         is_list(val->val.cons.cdr));
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

  .is_list = is_list,
  .list = {
    .len = list_len,
    .is_all = list_is_all,
  }
};
