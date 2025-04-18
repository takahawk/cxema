#include "cxema.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

	SValue *sval = cx->parse(cx, "(1 3 3 7)");

	assert_type_equals(SVAL_TYPE_CONS, sval, __FILE__, __LINE__);

  SValue *head = sval;
  SValue *car = head->val.cons.car;
  SValue *cdr = head->val.cons.cdr;
  assert_type_equals(SVAL_TYPE_CONS, cdr, __FILE__, __LINE__);
  assert_type_equals(SVAL_TYPE_INT, car, __FILE__, __LINE__);
  assert_long_equals(1, car->val._int);

  head = cdr;
  car = head->val.cons.car;
  cdr = head->val.cons.cdr;
  assert_type_equals(SVAL_TYPE_CONS, cdr, __FILE__, __LINE__);
  assert_type_equals(SVAL_TYPE_INT, car, __FILE__, __LINE__);
  assert_long_equals(3, car->val._int);

  head = cdr;
  car = head->val.cons.car;
  cdr = head->val.cons.cdr;
  assert_type_equals(SVAL_TYPE_CONS, cdr, __FILE__, __LINE__);
  assert_type_equals(SVAL_TYPE_INT, car, __FILE__, __LINE__);
  assert_long_equals(3, car->val._int);

  head = cdr;
  car = head->val.cons.car;
  cdr = head->val.cons.cdr;
  assert_null(cdr);
  assert_type_equals(SVAL_TYPE_INT, car, __FILE__, __LINE__);
  assert_long_equals(7, car->val._int);

  char *strval = SVALUE.to_string(sval);
	assert_str_equals("(1 . (3 . (3 . (7 . nil))))", strval, __FILE__, __LINE__);

  SVALUE.release(&sval);
  free(strval);
	cx->release(&cx);

	return 0;
}
