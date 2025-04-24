#include "cons.h"

#include "svalue.h"
#include <stdio.h>

static bool is_list(SValue *val) 
{
  return val->type == SVAL_TYPE_CONS ||
         val->val.cons.cdr == NULL ||
         is_list(val->val.cons.cdr);
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
  .is_list = is_list,
  .list = {
    .len = list_len,
    .is_all = list_is_all,
  }
};
