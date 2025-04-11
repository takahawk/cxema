#include "cxema.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

	SValue *sval = cx->parse(cx, "486");

	assert_type_equals(sval->type, SVAL_TYPE_NUM);
	assert_long_equals(sval->val.num, 486);

	cx->release(&cx);

	return 0;
}