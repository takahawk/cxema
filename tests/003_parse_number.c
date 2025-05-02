#include "cons.h"
#include "cxema.h"
#include "svalue.h"

#include "asserts.h"

int main()
{
	Cxema *cx = CXEMA.form();

	SValue *sval = CONS.list.take_first(cx->parse(cx, "; comment before \n"
                                                    "486              \n"
                                                    "; comment after"));

	assert_type_equals(SVAL_TYPE_INT, sval, __FILE__, __LINE__);
	assert_long_equals(486, sval->val._int);

  SVALUE.release(&sval);

	cx->release(&cx);

	return 0;
}
