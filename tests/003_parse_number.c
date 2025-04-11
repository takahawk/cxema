#include "cxema.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

	SValue *sval = cx->parse(cx, "486  ");

	assert_type_equals(SVAL_TYPE_NUM, sval->type);
	assert_long_equals(486, sval->val.num);

	cx->release(&cx);

	return 0;
}