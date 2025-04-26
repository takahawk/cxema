#include "cxema.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

  assert_interprets_as(cx, "(=)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1 1 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1 1 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1 1.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1.0 1.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(= 1.0 2.0)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);

  assert_interprets_as(cx, "(<)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1 2)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 2 1)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1 2 3)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1 2 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1.0 2.0 3.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1 2.0 3)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);

  assert_interprets_as(cx, "(<=)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1 2)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 2 1)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1 2 3)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1 1 2)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 1 2 1)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);

  assert_interprets_as(cx, "(>)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 2 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 1 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 3 2 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 3 2 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);

  assert_interprets_as(cx, "(>=)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 2 2)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 2 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 1 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 3 2 2 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 3 2 1 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 3 2 1 2)", SVAL_TYPE_BOOL, "#f", __FILE__, __LINE__);

  assert_interprets_as(cx, "(= 1 1.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(< 1 2.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(> 2.0 1)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(<= 2 2.0)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);
  assert_interprets_as(cx, "(>= 2.0 2)", SVAL_TYPE_BOOL, "#t", __FILE__, __LINE__);

	cx->release(&cx);

	return 0;
}
