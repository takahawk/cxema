#include "cxema.h"
#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

  assert_interprets_as(cx, "(define sqr (lambda (x) (* x x)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(sqr 15)", SVAL_TYPE_INT, "225", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define multiclause (lambda (x) (+ x 20) (+ 1000 x)))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(multiclause 337)", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);
  assert_interprets_as(cx, "(define parameterless (lambda () 1337))", SVAL_TYPE_VOID, "void", __FILE__, __LINE__);
  assert_interprets_as(cx, "(parameterless)", SVAL_TYPE_INT, "1337", __FILE__, __LINE__);

	cx->release(&cx);

	return 0;
}
