#include <stdio.h>
#include <stdlib.h>

#include "cons.h"
#include "cxema.h"
#include "svalue.h"

int interpret_file(const char *filename) {
	Cxema *cxema = CXEMA.form();

  SValue *result = cxema->interpret_file(cxema, filename);
  if (SVALUE.is_err(result)) {
    fprintf(stderr, "%s\n", SVALUE.to_string(result));
    SVALUE.release(&result);
    return -1;
  }
  SVALUE.release(&result);

	return 0;
}
