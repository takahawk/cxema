#include "cxema.h"
#include "svalue.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

  assert_interprets_as(cx, "486", SVAL_TYPE_NUM, "486");
  assert_interprets_as(cx, "(+ 137 349)", SVAL_TYPE_NUM, "486");
  assert_interprets_as(cx, "(- 1000 334)", SVAL_TYPE_NUM, "666");
  assert_interprets_as(cx, "(* 5 99)", SVAL_TYPE_NUM, "495");
  assert_interprets_as(cx, "(/ 10 5)", SVAL_TYPE_NUM, "2");
  assert_interprets_as(cx, "(+ 2.7 10)", SVAL_TYPE_NUM, "12.7");
  assert_interprets_as(cx, "(+ 21 35 12 7)", SVAL_TYPE_NUM, "75");
  assert_interprets_as(cx, "(* 25 4 12)", SVAL_TYPE_NUM, "1200");
  assert_interprets_as(cx, "(+ (* 3 5) (- 10 6))", SVAL_TYPE_NUM, "19");
  assert_interprets_as(cx, "(+ (* 3 (+ (* 2 4) (+ 3 5))) (+ (- 10 7) 5))",
                       SVAL_TYPE_NUM, "57");

	cx->release(&cx);

	return 0;
}
