#include "cxema.h"
#include "svalue.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

  assert_interprets_as(cx, "486", SVAL_TYPE_INT, "486", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 137 349)", SVAL_TYPE_INT, "486", __FILE__, __LINE__);
  assert_interprets_as(cx, "(- 1000 334)", SVAL_TYPE_INT, "666", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* 5 99)", SVAL_TYPE_INT, "495", __FILE__, __LINE__);
  assert_interprets_as(cx, "(/ 10 5)", SVAL_TYPE_INT, "2", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 2.7 10)", SVAL_TYPE_FLOAT, "12.7", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ 21 35 12 7)", SVAL_TYPE_INT, "75", __FILE__, __LINE__);
  assert_interprets_as(cx, "(* 25 4 12)", SVAL_TYPE_INT, "1200", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ (* 3 5) (- 10 6))", SVAL_TYPE_INT, "19", __FILE__, __LINE__);
  assert_interprets_as(cx, "(+ (* 3 (+ (* 2 4) (+ 3 5))) (+ (- 10 7) 6))",
                       SVAL_TYPE_INT, "57", __FILE__, __LINE__);

	cx->release(&cx);

	return 0;
}
