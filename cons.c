#include "cons.h"

#include "evaluator.h"
#include "svalue.h"

#include <stdio.h>
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

static SValue* list_reverse(SValue *list)
{
  SValue* head = list;
  SValue* reversed = NULL;

  while (head) {
    reversed = SVALUE.cons(CONS.car(head), reversed);
    SValue *next = CONS.cdr(head);
    free(head);
    head = next;
  }

  return reversed;
}

static void list_eval_items(SValue *list, Env *env)
{
  while (list) {
    list->val.cons.car = EVAL(env, CONS.car(list));
    list = CONS.cdr(list);
  }
}

static void list_println_items(SValue *list)
{
  while (list) {
    char *str = SVALUE.to_string(CONS.car(list));
    printf("%s\n", str);
    free(str);
    list = CONS.cdr(list);
  }
}

static void _release_sval(Allocator *a, void **sval)
{
  SValue *val = *(SValue **) sval;
  SVALUE.release(&val);
  *sval = NULL;
}

static Array* list_to_array(SValue *list)
{
  Array *array = ARRAY.form(sizeof(SValue*));
  array->release_cb = RELEASE_CB.form(&STD_ALLOCATOR, _release_sval);

  SValue *head = list;

  while (head) {
    SValue *car = CONS.car(head);
    SValue *cdr = CONS.cdr(head);
    free(head);
    array->add(array, &car);
    head = cdr;
  }

  return array;
}

static SValue* list_take_first(SValue *list)
{
  SValue *car = CONS.car(list);
  SValue *cdr = CONS.cdr(list);
  free(list);

  SVALUE.release(&cdr);

  return car;
}

static SValue* list_take_last(SValue *list)
{
  SValue *cdr;
  while (cdr = CONS.cdr(list)) {
    SValue *car = CONS.car(list);
    SVALUE.release(&car);
    free(list);
    list = cdr;
  }
  SValue *car = CONS.car(list);
  free(list);

  return car;
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
    .reverse = list_reverse,
    .eval_items = list_eval_items,
    .println_items = list_println_items,
    .to_array = list_to_array,

    .take_first = list_take_first,
    .take_last  = list_take_last,
  }
};
