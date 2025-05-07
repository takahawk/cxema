#include "cxema.h"
#include "asserts.h"
#include "execs/file_interpreter.h"

int main() {
  assert_long_equals(0, interpret_file("tests/008_tail_calls.scm"));
	return 0;
}
