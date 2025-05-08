#include "cxema.h"
#include "asserts.h"
#include "execs/file_interpreter.h"

int main() {
	Cxema *cx = CXEMA.form();
  SValue *sval = cx->interpret_file_all(cx, "tests/009_first_class_functions.scm");
	assert_type_equals(SVAL_TYPE_CONS, sval, __FILE__, __LINE__);

  Array *res = CONS.list.to_array(sval);

  assert_str_equals("8", SVALUE.to_string(*(SValue **) res->get(res, 2)), __FILE__, __LINE__);
  assert_str_equals("1337", SVALUE.to_string(*(SValue **) res->get(res, 3)), __FILE__, __LINE__);
  assert_str_equals("1337", SVALUE.to_string(*(SValue **) res->get(res, 5)), __FILE__, __LINE__);

  res->release(&res);
	return 0;
}
