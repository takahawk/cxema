#include "cxema.h"

#include "asserts.h"

int main() {
	Cxema *cx = CXEMA.form();

	SValue *sval = cx->parse(cx, "13371337133713371337");

	assert_type_equals(SVAL_TYPE_ERR, sval, __FILE__, __LINE__);

	SVALUE.release(&sval);

	cx->release(&cx);

	return 0;
}
