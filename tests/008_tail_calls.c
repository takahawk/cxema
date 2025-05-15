#include "cxema.h"
#include "asserts.h"
#include "execs/file_interpreter.h"

int main() {
	Cxema *cx = CXEMA.form();
  SValue *sval = cx->interpret_file(cx, "tests/008_tail_calls.scm");
  SVALUE.release(&sval);

  cx->release(&cx);
	return 0;
}
